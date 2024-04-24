`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:24:14 03/14/2021 
// Design Name: 
// Module Name:    pkt_classification 
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
module pkt_classification(ACLK,
								  ARESETN,
								  S_AXIS_TSTRB,
								  S_AXIS_TDATA,
								  timestamp_counter,
								  S_AXIS_TVALID,
								  S_AXIS_TLAST,
								  S_AXIS_TREADY,
								  five_tuple,
								  pkt_info,
								  tuple_and_info_valid,
								  num_processed_pkts
								  );
								  
parameter signed 		[63:0]   C_S_AXIS_10GMAC_DATA_WIDTH = 64;


input 						     ACLK;
input 						     ARESETN;
output reg 					     S_AXIS_TREADY;
input 					[63:0] 	 S_AXIS_TDATA;
input 					[7:0]  	 S_AXIS_TSTRB;
input 						     S_AXIS_TVALID;
input 						  	 S_AXIS_TLAST;

input 					[31:0] 	 timestamp_counter;

output 					[103:0]  five_tuple;
output reg 				[55:0]   pkt_info;
output 					[31:0]   num_processed_pkts;
output reg 				      	 tuple_and_info_valid;




wire 					[63:0]   S_AXIS_TDATA_rev;
reg 					[31:0]   processed_packets;
reg						      	 new_packet;
reg 					[15:0]   frame_ip_total_length;
reg 					[7:0]    frame_tcp_flags;
reg 					[31:0]   frame_timestamp;
reg 					[31:0]   src_ip;
reg 					[31:0]   dest_ip;
reg 					[15:0]   src_port;
reg 					[15:0]   dest_port;
reg 					[7:0]    protocol;
reg 					[4:0]    extract_fsm;


localparam  IDLE_STATE = 5'b00000,
				PASS_PKT = 5'b00001,
				DONT_TRANSMIT_STATE = 5'b00010,
				TRANSMIT_STATE = 5'b00011,
				RCV_1 = 5'b00100,
				RCV_ipv4_no_vlan_0 = 5'b00101,
				RCV_ipv4_no_vlan_1 = 5'b00110,
				RCV_ipv4_no_vlan_2 = 5'b00111,
				RCV_ipv4_no_vlan_3 = 5'b01000,
				RCV_ipv4_vlan1_0 = 5'b01001,
				RCV_ipv4_vlan1_1 = 5'b01010,
				RCV_ipv4_vlan1_2 = 5'b01011,
				RCV_ipv4_vlan1_3 = 5'b01100,
				RCV_ipv4_vlan1_4 = 5'b01101,
				RCV_ipv4_vlan2_0 = 5'b01110,
				RCV_ipv4_vlan2_1 = 5'b01111,
				RCV_ipv4_vlan2_2 = 5'b10000,
				RCV_ipv4_vlan2_3 = 5'b10001;

assign num_processed_pkts = processed_packets;
assign five_tuple = {src_ip, dest_ip, src_port, dest_port, protocol};
assign S_AXIS_TDATA_rev = {S_AXIS_TDATA [7:0], S_AXIS_TDATA [15:8], S_AXIS_TDATA [23:16], S_AXIS_TDATA [31:24], S_AXIS_TDATA [39:32], S_AXIS_TDATA [47:40], S_AXIS_TDATA [55:48], S_AXIS_TDATA [63:56]}; // them vao thi k chay isim

always @(posedge ACLK or negedge ARESETN)  begin
	 if (!ARESETN) begin
	  new_packet <= 1'b1; 
	  S_AXIS_TREADY <= 1'b0;
	  processed_packets <= 32'b0; 
	  tuple_and_info_valid <= 1'b0;
	  extract_fsm <= IDLE_STATE;
	 end
	 else begin
	  S_AXIS_TREADY <= 1'b1;
	  tuple_and_info_valid <= 1'b0;
	  if (S_AXIS_TVALID && new_packet) begin
	   new_packet <= 1'b0;
	   frame_timestamp <= timestamp_counter; 
	  end
	  case (extract_fsm)
	  //integer extract_fsm;
		IDLE_STATE	:begin 
						if (S_AXIS_TVALID) begin
					    extract_fsm <= RCV_1;
					end
					end
		RCV_1   	: begin 
						processed_packets <= processed_packets + 1'b1; 
						if (S_AXIS_TVALID) begin
							if (S_AXIS_TDATA_rev[31:16] == 16'h0800 && S_AXIS_TDATA_rev[15:12] == 4'h4) begin
								extract_fsm <= RCV_ipv4_no_vlan_0;
							end
							else if (S_AXIS_TDATA_rev[31:16] == 16'h0800) begin
								extract_fsm <= RCV_ipv4_vlan1_0;
							end
							else 
								extract_fsm <= DONT_TRANSMIT_STATE;
						end
						end
		RCV_ipv4_no_vlan_0 : begin
						frame_ip_total_length[15:0]  <= S_AXIS_TDATA_rev[63:48];
						protocol[7:0] <= S_AXIS_TDATA_rev[7:0]; 
						if (S_AXIS_TVALID) begin
							if (S_AXIS_TDATA_rev[7:0] == 8'h06 || S_AXIS_TDATA_rev[7:0] == 8'h11)  begin
								extract_fsm <= RCV_ipv4_no_vlan_1;
								end
							else 
								extract_fsm <= DONT_TRANSMIT_STATE;
						end
					end
		RCV_ipv4_no_vlan_1 : begin
						src_ip[31:0] <= S_AXIS_TDATA_rev[47:16]; 
						dest_ip[31:16] <= S_AXIS_TDATA_rev[15:0]; 
						if (S_AXIS_TVALID) 
							extract_fsm <= RCV_ipv4_no_vlan_2;
					end
		RCV_ipv4_no_vlan_2 : begin
						dest_ip[15:0] <= S_AXIS_TDATA_rev[63:48]; 
						src_port[15:0] <= S_AXIS_TDATA_rev[47:32]; 
						dest_port[15:0] <= S_AXIS_TDATA_rev [31:16]; 
						if (S_AXIS_TVALID) 
							extract_fsm <=  RCV_ipv4_no_vlan_3;
					end
		 RCV_ipv4_no_vlan_3 : begin
						if (protocol == 8'h06) begin
							frame_tcp_flags[7:0] <= S_AXIS_TDATA_rev[7:0];
							end
						else 
							frame_tcp_flags[7:0] <= 8'h00; 
						if (S_AXIS_TVALID) begin
							 extract_fsm <= TRANSMIT_STATE; 
						end
					end
						
		 RCV_ipv4_vlan1_0 : begin
					frame_ip_total_length[7:0] <= S_AXIS_TDATA_rev[31:16];
						if (S_AXIS_TVALID) begin
						end
							if (S_AXIS_TDATA_rev[63:48] == 16'h8100 && S_AXIS_TDATA_rev[31:16] == 16'h0800 && S_AXIS_TDATA_rev[15:12] == 4'h4) begin
								extract_fsm <= RCV_ipv4_vlan2_0;
							end
							else if (S_AXIS_TDATA_rev[63:48] == 16'h0800 && S_AXIS_TDATA_rev[47:44] == 4'h4) 
								extract_fsm <= RCV_ipv4_vlan1_1; 
							else 
								extract_fsm <= DONT_TRANSMIT_STATE; 
						end
		RCV_ipv4_vlan1_1 : begin
					protocol[7:0] <= S_AXIS_TDATA_rev[39:32];
					src_ip[31:16] <= S_AXIS_TDATA_rev[15:0];
					if (S_AXIS_TVALID) 
						extract_fsm <= RCV_ipv4_vlan1_2 ; 
					end
		RCV_ipv4_vlan1_2 	: begin
					src_ip[15:0] <= S_AXIS_TDATA_rev[63:48];
					dest_ip[31:0] <= S_AXIS_TDATA_rev[47:32];
					src_port[15:0] <= S_AXIS_TDATA_rev[31:16];
					if (S_AXIS_TVALID) 
						extract_fsm <= RCV_ipv4_vlan1_3; 
					end
		RCV_ipv4_vlan1_3 : begin
					dest_port[15:0] <= S_AXIS_TDATA_rev[63:48];
					if (S_AXIS_TVALID) 
						extract_fsm <= RCV_ipv4_vlan1_4;
					end
		RCV_ipv4_vlan1_4 : begin
					if (protocol == 8'h06) 
						frame_tcp_flags[7:0] <= S_AXIS_TDATA_rev[7:0];
					else
						frame_tcp_flags[7:0] <= 8'h00; 
					if (S_AXIS_TVALID)
						extract_fsm <= TRANSMIT_STATE;
					end
		RCV_ipv4_vlan2_0 : begin
					frame_ip_total_length[15:0] <= S_AXIS_TDATA_rev[63:48];
					protocol[7:0] <= S_AXIS_TDATA_rev[63:48];
					if (S_AXIS_TVALID)
						extract_fsm <= RCV_ipv4_vlan2_1;
					end
		RCV_ipv4_vlan2_1 : begin
					src_ip[31:0] <= S_AXIS_TDATA_rev[47:16];
					dest_ip[31:16] <= S_AXIS_TDATA_rev[15:0];
					if (S_AXIS_TVALID) 
						extract_fsm <= RCV_ipv4_vlan2_2;
					end
		RCV_ipv4_vlan2_2 : begin
					dest_ip[15:0] <= S_AXIS_TDATA_rev[63:48];
					src_port[15:0] <= S_AXIS_TDATA_rev[47:32];
					dest_port[15:0] <= S_AXIS_TDATA_rev[31:16];
					if (S_AXIS_TVALID) 
						extract_fsm <= RCV_ipv4_vlan2_3;
					end
		RCV_ipv4_vlan2_3 : begin
					if (protocol == 8'h06) 
						frame_tcp_flags[7:0] <= S_AXIS_TDATA_rev[7:0];
					else
						frame_tcp_flags[7:0] <= 8'h00; 
					if (S_AXIS_TVALID) 
						extract_fsm <= TRANSMIT_STATE;
					end
		DONT_TRANSMIT_STATE : begin
					if (S_AXIS_TVALID && S_AXIS_TLAST) 
						extract_fsm <= IDLE_STATE;
						new_packet <= 1'b1;
					end
		TRANSMIT_STATE : begin
					pkt_info[15:0] <= frame_ip_total_length[15:0];
					pkt_info[47:16] <= frame_timestamp[31:0];
					pkt_info[55:48] <= frame_tcp_flags[7:0];
					if (S_AXIS_TVALID && S_AXIS_TLAST) 
						new_packet <= 1'b1;
						extract_fsm <= IDLE_STATE; 
						tuple_and_info_valid <= 1'b1; 
					end
		default : begin
				extract_fsm <= DONT_TRANSMIT_STATE;
					end
				
					endcase

end
end
endmodule
