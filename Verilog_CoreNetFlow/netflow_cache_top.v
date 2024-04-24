`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    09:12:15 03/15/2021 
// Design Name: 
// Module Name:    uut 
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
module netflow_cache(	ACLK,
						ARESETN,
						S_AXIS_TDATA,
						S_AXIS_TSTRB,
						S_AXIS_TLAST,
						S_AXIS_TVALID,
						S_AXIS_TREADY,
						M_AXIS_EXP_RECORDS_TREADY,
						M_AXIS_EXP_RECORDS_TDATA,
						M_AXIS_EXP_RECORDS_TSTRB,
						M_AXIS_EXP_RECORDS_TVALID,
						M_AXIS_EXP_RECORDS_TLAST );
//Input
input ACLK;
input ARESETN;
input [63:0]S_AXIS_TDATA;	
input [7:0]S_AXIS_TSTRB;
input S_AXIS_TLAST;
input S_AXIS_TVALID;
input M_AXIS_EXP_RECORDS_TREADY;	

// output
output S_AXIS_TREADY;
output [63:0] M_AXIS_EXP_RECORDS_TDATA;
output [7:0]  M_AXIS_EXP_RECORDS_TSTRB;
output M_AXIS_EXP_RECORDS_TVALID;
output M_AXIS_EXP_RECORDS_TLAST;

// variable
parameter SIM_ONLY = 0;
parameter ACLK_FREQ = 200000000;
parameter C_S_AXIS_10GMAC_DATA_WIDTH = 64;
parameter C_M_AXIS_EXP_RECORDS_DATA_WIDTH = 64;
parameter C_ACTIVE_TIMEOUT_INIT = 1500;
parameter C_InACTIVE_TIMEOUT_INIT = 1;	
parameter NETFLOW_EXPORT_PRESENT = 1'b1; 

//signal 
// time_out
wire 	  [31:0]ACTIVE_TIMEOUT;
wire      [31:0]InACTIVE_TIMEOUT;
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
//ful
wire [3:0]fifo_full_exp_int;
wire fifo_full_exp;
wire [3:0]fifo_empty_exp_int;
wire fifo_empty_exp;
wire fifo_rd_exp_en;
wire fifo_exp_rst;
wire fifo_w_exp_en;

wire [8 :0] RDCOUNT_exp[0:3];
wire [8 :0] WRCOUNT_exp[0:3];


begin

assign ACTIVE_TIMEOUT   = (SIM_ONLY == 0) ? ACTIVE_TIMEOUT_HW_IMP : ACTIVE_TIMEOUT_SYM;
assign InACTIVE_TIMEOUT = (SIM_ONLY == 0) ? InACTIVE_TIMEOUT_HW_IMP : InACTIVE_TIMEOUT_SYM;


pkt_classification classification(

	.ACLK    				(ACLK),
	.ARESETN 				(ARESETN),
	.S_AXIS_TREADY 			(S_AXIS_TREADY),
	.S_AXIS_TDATA  			(S_AXIS_TDATA),
	.S_AXIS_TSTRB  			(S_AXIS_TSTRB),
	.S_AXIS_TLAST 			(S_AXIS_TLAST),
	.S_AXIS_TVALID 			(S_AXIS_TVALID),
	.timestamp_counter  	(timestamp_counter),
	.num_processed_pkts 	(num_processed_pkts),
	.five_tuple 			(frame_five_tuple),
	.pkt_info  				(pkt_info),
	.tuple_and_info_valid 	(tuple_and_info_valid)
	 );
	//time stamp
 timestamp_counter_mod timestamp(

	.ACLK    				(ACLK),
	.ARESETN 				(ARESETN),
	.timestamp_counter_out 	(timestamp_counter)
    );	
/////
 BSRAM flow_table  ( 
	.clk   (ACLK),
	.ena   (ena),
	.enb   (enb),
	.wea   (wea),
	.web   (web),
	.addra (addra),
	.addrb (addrb),
	.dia   (dia),
	.dib   (dib),
	.doa   (doa),
	.dob   (dob)
	 );
// create or update
create_or_update_flows create_or_update (

	.ACLK    				(ACLK),
	.ARESETN 				(ARESETN),
	.frame_five_tuple  		(frame_five_tuple),
	.pkt_info 				(pkt_info),
	.tuple_and_info_valid 	(tuple_and_info_valid),
	.ena 					(ena),
	.wea 					(wea),
	.hash_code_out 			(addra),
	.doa 					(doa),
	.dia 					(dia),
	.export_now 			(export_now),
	.export_this 			(export_this),
	.flow_exported_ok  		(flow_exported_ok),
	.collision_counter 		(collision_counter)
	 );

	
// exp flows fr memory
export_expired_flows_from_mem export_expired_flows_fr_mem  (
	
	.ACLK    			(ACLK),
	.ARESETN 			(ARESETN),
	.ACTIVE_TIMEOUT   	(ACTIVE_TIMEOUT),
	.InACTIVE_TIMEOUT 	(InACTIVE_TIMEOUT),
	.enb 				(enb),
	.web 				(web),
	.addrb 				(addrb),
	.dob 				(dob),
	.dib 				(dib),
	.export_now  		(export_now),
	.export_this 		(export_this),
	.timestamp_counter 	(timestamp_counter),
	.flow_exported_ok  	(flow_exported_ok),
	.fifo_exp_rst  		(fifo_exp_rst),
	.fifo_w_exp_en 		(fifo_w_exp_en),
	.fifo_in_exp   		(fifo_in_exp),
	.fifo_full_exp 		(fifo_full_exp)
	);
	
	// fifo 
	
 
FIFO_SYNC_MACRO #(
		.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
		.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
		.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
		.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")

		//.DO_REG (0),
		.FIFO_SIZE ("36Kb"), 								// Target BRAM, "18Kb" or "36Kb"
		.SIM_MODE ("SAFE") 									// Simulation) "SAFE" vs "FAST",
// see "Synthesis and Simulation Design Guide" for details
		) 
FIFO_SYNC_MACRO_exp0
		(
		.ALMOSTEMPTY (), 								// Output almost empty
		.ALMOSTFULL  (fifo_full_exp_int[0]),		// Output almost full
		.DO    (fifo_out_exp_int[0]), 				// Output data
		.EMPTY (fifo_empty_exp_int[0]), 			// Output empty
		.FULL    (), 										// Output full
		.RDCOUNT (RDCOUNT_exp[0]), 				// Output read count
		.RDERR   (), 									// Output read error
		.WRCOUNT (WRCOUNT_exp[0]), 				// Output write count
		.WRERR(), 										// Output write error
		.CLK  (ACLK), 									// Input clock
		.DI   (fifo_in_exp_int[0]), 				// Input data
		.RDEN (fifo_rd_exp_en), 					// Input read enable
		.RST  (fifo_exp_rst), 						// Input reset
		.WREN (fifo_w_exp_en) 						// Input write enable
		);
		
FIFO_SYNC_MACRO #(
		.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
		.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
		.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
		.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")

		//.DO_REG (0),
		.FIFO_SIZE ("36Kb"), 								// Target BRAM, "18Kb" or "36Kb"
		.SIM_MODE ("SAFE") 									// Simulation) "SAFE" vs "FAST",
// see "Synthesis and Simulation Design Guide" for details
		) 
FIFO_SYNC_MACRO_exp1
		(
		.ALMOSTEMPTY (), 								// Output almost empty
		.ALMOSTFULL  (fifo_full_exp_int[1]),		// Output almost full
		.DO    (fifo_out_exp_int[1]), 				// Output data
		.EMPTY (fifo_empty_exp_int[1]), 			// Output empty
		.FULL    (), 										// Output full
		.RDCOUNT (RDCOUNT_exp[1]), 				// Output read count
		.RDERR   (), 									// Output read error
		.WRCOUNT (WRCOUNT_exp[1]), 				// Output write count
		.WRERR(), 										// Output write error
		.CLK  (ACLK), 									// Input clock
		.DI   (fifo_in_exp_int[1]), 				// Input data
		.RDEN (fifo_rd_exp_en), 					// Input read enable
		.RST  (fifo_exp_rst), 						// Input reset
		.WREN (fifo_w_exp_en) 						// Input write enable
		);
		
FIFO_SYNC_MACRO #(
		.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
		.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
		.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
		.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")

		//.DO_REG (0),
		.FIFO_SIZE ("36Kb"), 								// Target BRAM, "18Kb" or "36Kb"
		.SIM_MODE ("SAFE") 									// Simulation) "SAFE" vs "FAST",
// see "Synthesis and Simulation Design Guide" for details
		) 
FIFO_SYNC_MACRO_exp2
		(
		.ALMOSTEMPTY (), 								// Output almost empty
		.ALMOSTFULL  (fifo_full_exp_int[2]),		// Output almost full
		.DO    (fifo_out_exp_int[2]), 				// Output data
		.EMPTY (fifo_empty_exp_int[2]), 			// Output empty
		.FULL    (), 										// Output full
		.RDCOUNT (RDCOUNT_exp[2]), 				// Output read count
		.RDERR   (), 									// Output read error
		.WRCOUNT (WRCOUNT_exp[2]), 				// Output write count
		.WRERR(), 										// Output write error
		.CLK  (ACLK), 									// Input clock
		.DI   (fifo_in_exp_int[2]), 				// Input data
		.RDEN (fifo_rd_exp_en), 					// Input read enable
		.RST  (fifo_exp_rst), 						// Input reset
		.WREN (fifo_w_exp_en) 						// Input write enable
		);
	
FIFO_SYNC_MACRO #(
		.DEVICE ("VIRTEX5"), 						// Target Device: "VIRTEX5, "VIRTEX6"
		.ALMOST_FULL_OFFSET (16'h0080), 			// Sets almost full threshold
		.ALMOST_EMPTY_OFFSET (16'h0080),			// Sets the almost empty threshold
		.DATA_WIDTH (72), 							// Valid values are 1-72 (37-72 only valid when FIFO_SIZE="36Kb")

		//.DO_REG (0),
		.FIFO_SIZE ("36Kb"), 								// Target BRAM, "18Kb" or "36Kb"
		.SIM_MODE ("SAFE") 									// Simulation) "SAFE" vs "FAST",
// see "Synthesis and Simulation Design Guide" for details
		) 
FIFO_SYNC_MACRO_exp3
		(
		.ALMOSTEMPTY (), 								// Output almost empty
		.ALMOSTFULL  (fifo_full_exp_int[3]),		// Output almost full
		.DO    (fifo_out_exp_int[3]), 				// Output data
		.EMPTY (fifo_empty_exp_int[3]), 			// Output empty
		.FULL    (), 										// Output full
		.RDCOUNT (RDCOUNT_exp[3]), 				// Output read count
		.RDERR   (), 									// Output read error
		.WRCOUNT (WRCOUNT_exp[3]), 				// Output write count
		.WRERR(), 										// Output write error
		.CLK  (ACLK), 									// Input clock
		.DI   (fifo_in_exp_int[3]), 				// Input data
		.RDEN (fifo_rd_exp_en), 					// Input read enable
		.RST  (fifo_exp_rst), 						// Input reset
		.WREN (fifo_w_exp_en) 						// Input write enable
		);
		
 
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
.ACLK  					(ACLK),
.ARESETN 				(ARESETN),
.M_AXIS_10GMAC_tdata 	(M_AXIS_EXP_RECORDS_TDATA),
.M_AXIS_10GMAC_tstrb 	(M_AXIS_EXP_RECORDS_TSTRB),
.M_AXIS_10GMAC_tvalid 	(M_AXIS_EXP_RECORDS_TVALID),
.M_AXIS_10GMAC_tready 	(M_AXIS_EXP_RECORDS_TREADY),
.M_AXIS_10GMAC_tlast 	(M_AXIS_EXP_RECORDS_TLAST),
.counters 				(num_processed_pkts),
.collision_counter 		(collision_counter),
.fifo_rd_exp_en 		(fifo_rd_exp_en),
.fifo_out_exp 			(fifo_out_exp),
.fifo_empty_exp 		(fifo_empty_exp));
// exp to netflow
/*exp_to_netflow_exp exp_to_netflow_exp_inst(		
.ACLK  						(ACLK),
.ARESETN 					(ARESETN),
.M_AXIS_10GMAC_tdata 	(M_AXIS_EXP_RECORDS_TDATA),
.M_AXIS_10GMAC_tstrb 	(M_AXIS_EXP_RECORDS_TSTRB),
.M_AXIS_10GMAC_tvalid 	(M_AXIS_EXP_RECORDS_TVALID),
.M_AXIS_10GMAC_tready 	(M_AXIS_EXP_RECORDS_TREADY),
.M_AXIS_10GMAC_tlast 	(M_AXIS_EXP_RECORDS_TLAST),
.fifo_rd_exp_en 			(fifo_rd_exp_en),
.fifo_out_exp 				(fifo_out_exp),
.fifo_empty_exp 			(fifo_empty_exp));*/
end

endmodule
