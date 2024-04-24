`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:52:02 03/18/2021 
// Design Name: 
// Module Name:    netflow_cache 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module netflow_cache(ACLK,
	ARESETN,
	S_AXIS_TREADY,
	S_AXIS_TDATA,
	S_AXIS_TSTRB,
	S_AXIS_TLAST,
	S_AXIS_TVALID,
	M_AXIS_EXP_RECORDS_TDATA,
	M_AXIS_EXP_RECORDS_TSTRB,
	M_AXIS_EXP_RECORDS_TVALID,
	M_AXIS_EXP_RECORDS_TLAST,
	M_AXIS_EXP_RECORDS_TREADY
    );
	 
// input
input ACLK;
input ARESETN;
input [63:0]S_AXIS_TDATA;
input [7:0]S_AXIS_TSTRB;
input S_AXIS_TLAST;
input S_AXIS_TVALID;
input M_AXIS_EXP_RECORDS_TREADY;

// output
output S_AXIS_TREADY;
output [63:0]M_AXIS_EXP_RECORDS_TDATA;
output [7:0]M_AXIS_EXP_RECORDS_TSTRB;
output M_AXIS_EXP_RECORDS_TVALID;
output M_AXIS_EXP_RECORDS_TLAST;

// variable
parameter SIM_ONLY = 0;
parameter ACLK_FREQ = 200000000;
parameter C_S_AXIS_10GMAC_DATA_WIDTH = 64;
parameter C_M_AXIS_EXP_RECORDS_DATA_WIDTH = 64;
parameter C_ACTIVE_TIMEOUT_INIT = 1500;	//<<
parameter C_InACTIVE_TIMEOUT_INIT = 1;	//<<
parameter NETFLOW_EXPORT_PRESENT = 1'b1; // <<

//signal 
// time_out
wire [31:0]ACTIVE_TIMEOUT;
wire [31:0]InACTIVE_TIMEOUT;
parameter [31:0]ACTIVE_TIMEOUT_HW_IMP = 1500000;
parameter [31:0]InACTIVE_TIMEOUT_HW_IMP = 100;
parameter [31:0]ACTIVE_TIMEOUT_SYM = 2;
parameter [31:0]InACTIVE_TIMEOUT_SYM = 1;

//packet classification
wire [103:0]frame_five_tuple; 
wire [55:0]pkt_info; 
wire [31:0]num_processed_pkts;
wire tuple_and_info_valid;
wire [31:0]timestamp_counter;

// flow table port A
wire ena;
wire wea;
wire [11:0]addra;
wire [240:0]dia;
wire [240:0]doa;

// flow table port B
wire enb;
wire web;
wire [11:0]addrb;
wire [240:0]dib;
wire [240:0]dob;

// create or update
wire export_now;
wire [11:0]export_this;
wire flow_exported_ok;
wire [31:0]collision_counter;

//fifo signal
wire [239:0]fifo_out_exp;
wire [239:0]fifo_in_exp;
wire [72-1 :0] fifo_out_exp_int [0:3];
wire [72-1 :0] fifo_in_exp_int  [0:3];
//full & empty
wire [3:0]fifo_full_exp_int;
wire fifo_full_exp;
wire [3:0]fifo_empty_exp_int;
wire fifo_empty_exp;
wire fifo_rd_exp_en;
wire fifo_exp_rst;
wire fifo_w_exp_en;

wire [8 :0] RDCOUNT_exp[0:3];
wire [8 :0] WRCOUNT_exp[0:3];

assign ACTIVE_TIMEOUT = (SIM_ONLY == 0) ? ACTIVE_TIMEOUT_HW_IMP : ACTIVE_TIMEOUT_SYM;
assign InACTIVE_TIMEOUT = (SIM_ONLY == 0) ? InACTIVE_TIMEOUT_HW_IMP : InACTIVE_TIMEOUT_SYM;
//
begin



pkt_classification classification(
	.ACLK(ACLK),
	.ARESETN(ARESETN),
	.S_AXIS_TREADY(S_AXIS_TREADY),
	.S_AXIS_TDATA(S_AXIS_TDATA),
	.S_AXIS_TSTRB(S_AXIS_TSTRB),
	.S_AXIS_TLAST(S_AXIS_TLAST),
	.S_AXIS_TVALID(S_AXIS_TVALID),
	.timestamp_counter(timestamp_counter),
	.num_processed_pkts(num_processed_pkts),
	.five_tuple(frame_five_tuple),
	.pkt_info(pkt_info),
	.tuple_and_info_valid(tuple_and_info_valid));
	//time stamp
timestamp_counter_mod #(.SIM_ONLY(SIM_ONLY),
								.ACLK_FREQ(ACLK_FREQ)) 
								timestamp(.ACLK(ACLK),
											.ARESETN(ARESETN),
											.timestamp_counter_out(timestamp_counter));
//flow-table
BSRAM flow_table(
	.clk(ACLK),
	.ena(ena),
	.enb(enb),
	.wea(wea),
	.web(web),
	.addra(addra),
	.addrb(addrb),
	.dia(dia),
	.dib(dib),
	.doa(doa),
	.dob(dob));
// create or update
create_or_update_flows create_or_update(
	.ACLK(ACLK),
	.ARESETN(ARESETN),
	.frame_five_tuple(frame_five_tuple),
	.pkt_info(pkt_info),
	.tuple_and_info_valid(tuple_and_info_valid),
	.ena(ena),
	.wea(wea),
	.hash_code_out(addra),
	.doa(doa),
	.dia(dia),
	.export_now(export_now),
	.export_this(export_this),
	.flow_exported_ok(flow_exported_ok),
	.collision_counter(collision_counter));
	
// exp flows fr memory
export_expired_flows_from_mem export_expired_flows_fr_mem(
	.ACLK(ACLK),
	.ARESETN(ARESETN),
	.ACTIVE_TIMEOUT(ACTIVE_TIMEOUT),
	.InACTIVE_TIMEOUT(InACTIVE_TIMEOUT),
	.enb(enb),
	.web(web),
	.addrb(addrb),
	.dob(dob),
	.dib(dib),
	.export_now(export_now),
	.export_this(export_this),
	.timestamp_counter(timestamp_counter),
	.flow_exported_ok(flow_exported_ok),
	.fifo_exp_rst(fifo_exp_rst),
	.fifo_w_exp_en(fifo_w_exp_en),
	.fifo_in_exp(fifo_in_exp),
	.fifo_full_exp(fifo_full_exp));
	
	// fifo 
	
 
FIFO_SYNC_MACRO #(
	.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
	.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
	.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
	.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
		//.DO_REG (0),
	.FIFO_SIZE ("36Kb"), 						// Target BRAM, "18Kb" or "36Kb"
	.SIM_MODE ("SAFE"))	
FIFO_SYNC_MACRO_exp0(
	.ALMOSTEMPTY(open),
	.ALMOSTFULL(fifo_full_exp_int[0]),
	.DO(fifo_out_exp_int[0]),
	.EMPTY(fifo_empty_exp_int[0]),
	.FULL(),
	.RDCOUNT(RDCOUNT_exp[0]),
	.RDERR(),
	.WRCOUNT(WRCOUNT_exp[0]),
	.WRERR(),
	.CLK(ACLK),
	.DI(fifo_in_exp_int[0]),
	.RDEN(fifo_rd_exp_en),
	.RST(fifo_exp_rst),
	.WREN(fifo_w_exp_en));
	
//1
FIFO_SYNC_MACRO #(
	.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
	.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
	.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
	.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
	//.DO_REG (0),
	.FIFO_SIZE ("36Kb"), 						// Target BRAM, "18Kb" or "36Kb"
	.SIM_MODE ("SAFE"))	
FIFO_SYNC_MACRO_exp1(
	.ALMOSTEMPTY(),
	.ALMOSTFULL(fifo_full_exp_int[1]),
	.DO(fifo_out_exp_int[1]),
	.EMPTY(fifo_empty_exp_int[1]),
	.FULL(),
	.RDCOUNT(RDCOUNT_exp[1]),
	.RDERR(),
	.WRCOUNT(WRCOUNT_exp[1]),
	.WRERR(),
	.CLK(ACLK),
	.DI(fifo_in_exp_int[1]),
	.RDEN(fifo_rd_exp_en),
	.RST(fifo_exp_rst),
	.WREN(fifo_w_exp_en));
//2
FIFO_SYNC_MACRO #(
	.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
	.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
	.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
	.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
	//.DO_REG (0),
	.FIFO_SIZE ("36Kb"), 						// Target BRAM, "18Kb" or "36Kb"
	.SIM_MODE ("SAFE"))	
FIFO_SYNC_MACRO_exp2(
	.ALMOSTEMPTY(),
	.ALMOSTFULL(fifo_full_exp_int[2]),
	.DO(fifo_out_exp_int[2]),
	.EMPTY(fifo_empty_exp_int[2]),
	.FULL(),
	.RDCOUNT(RDCOUNT_exp[2]),
	.RDERR(),
	.WRCOUNT(WRCOUNT_exp[2]),
	.WRERR(),
	.CLK(ACLK),
	.DI(fifo_in_exp_int[2]),
	.RDEN(fifo_rd_exp_en),
	.RST(fifo_exp_rst),
	.WREN(fifo_w_exp_en));
FIFO_SYNC_MACRO #(
	.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
	.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
	.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
	.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")
	//.DO_REG (0),
	.FIFO_SIZE ("36Kb"), 						// Target BRAM, "18Kb" or "36Kb"
	.SIM_MODE ("SAFE"))	
FIFO_SYNC_MACRO_exp3(
	.ALMOSTEMPTY(),
	.ALMOSTFULL(fifo_full_exp_int[3]),
	.DO(fifo_out_exp_int[3]),
	.EMPTY(fifo_empty_exp_int[3]),
	.FULL(),
	.RDCOUNT(RDCOUNT_exp[3]),
	.RDERR(),
	.WRCOUNT(WRCOUNT_exp[3]),
	.WRERR(),
	.CLK(ACLK),
	.DI(fifo_in_exp_int[3]),
	.RDEN(fifo_rd_exp_en),
	.RST(fifo_exp_rst),
	.WREN(fifo_w_exp_en));
// asign fifo	

assign fifo_empty_exp = fifo_empty_exp_int[3] || fifo_empty_exp_int[2] || fifo_empty_exp_int[1] ||fifo_empty_exp_int[0];
assign fifo_full_exp 	= fifo_full_exp_int[3] || fifo_full_exp_int[2] || fifo_full_exp_int[1] || fifo_full_exp_int[0];
assign fifo_out_exp	=	{fifo_out_exp_int[3][23:0], fifo_out_exp_int[2], fifo_out_exp_int[1], fifo_out_exp_int[0]};
assign fifo_in_exp_int[0] = fifo_in_exp[71:0];
assign fifo_in_exp_int[1] = fifo_in_exp[143:72];
assign fifo_in_exp_int[2] = fifo_in_exp[215:144];
assign fifo_in_exp_int[3] = {48'b0, fifo_in_exp[239 : 216]};

// exp 10g
exp_via_10g_interface exp_via_10g_intf_inst( 	
	.ACLK  						(ACLK),
	.ARESETN 					(ARESETN),
	.M_AXIS_10GMAC_tdata 	(M_AXIS_EXP_RECORDS_TDATA),
	.M_AXIS_10GMAC_tstrb 	(M_AXIS_EXP_RECORDS_TSTRB),
	.M_AXIS_10GMAC_tvalid 	(M_AXIS_EXP_RECORDS_TVALID),
	.M_AXIS_10GMAC_tready 	(M_AXIS_EXP_RECORDS_TREADY),
	.M_AXIS_10GMAC_tlast 	(M_AXIS_EXP_RECORDS_TLAST),
	.counters 					(num_processed_pkts),
	.collision_counter 		(collision_counter),
	.fifo_rd_exp_en 			(fifo_rd_exp_en),
	.fifo_out_exp 				(fifo_out_exp),
	.fifo_empty_exp 			(fifo_empty_exp));
// exp to netflow
exp_to_netflow_exp exp_to_netflow_exp_inst(		
	.ACLK  						(ACLK),
	.ARESETN 					(ARESETN),
	.M_AXIS_10GMAC_tdata 	(M_AXIS_EXP_RECORDS_TDATA),
	.M_AXIS_10GMAC_tstrb 	(M_AXIS_EXP_RECORDS_TSTRB),
	.M_AXIS_10GMAC_tvalid 	(M_AXIS_EXP_RECORDS_TVALID),
	.M_AXIS_10GMAC_tready 	(M_AXIS_EXP_RECORDS_TREADY),
	.M_AXIS_10GMAC_tlast 	(M_AXIS_EXP_RECORDS_TLAST),
	.fifo_rd_exp_en 			(fifo_rd_exp_en),
	.fifo_out_exp 				(fifo_out_exp),
	.fifo_empty_exp 			(fifo_empty_exp));
end

endmodule
