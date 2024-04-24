`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:01:59 03/15/2021 
// Design Name: 
// Module Name:    create_or_update_flows 
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

module create_or_update_flows(ACLK,
			ARESETN,
			frame_five_tuple,
			pkt_info,
			tuple_and_info_valid,
			flow_exported_ok,
			doa,
			ena,
			wea,
			hash_code_out,
			dia,
			export_now,
			export_this,
			collision_counter
			);

	input ACLK;
	input ARESETN;
	input [103:0] frame_five_tuple;
	input [55:0] pkt_info;
	input  tuple_and_info_valid;
	input [240:0] doa;
	input flow_exported_ok;
	// out put
	output 	  [31:0] collision_counter;
	output reg [240:0] dia;
	output reg [11:0] export_this;
	output	  [11:0] hash_code_out;
	output reg ena;
	output reg wea;
	output reg export_now;
	// wire and reg
	reg [103: 0] frame_five_tuple_reg;
	wire [11: 0] hash_code;
	reg [15 : 0] frame_ip_total_length;
	reg [31 : 0] frame_timestamp;
	reg [7  : 0] frame_tcp_flags;
	reg [31 : 0] flow_pkt_counter;
	reg [31 : 0] flow_byte_counter;
	reg [7  : 0] flow_tcp_flags;
	reg [31 : 0] flow_initial_timestamp;
	reg [31 : 0] flow_last_timestamp;
	reg [240: 0] reg_doa;
	reg [31 : 0] collision_counter_int;
	reg [7  : 0] protocol;
	reg [103: 0] flow_5_tuple;
	reg fin_rst_flags;
// assign
	assign collision_counter = collision_counter_int;
	assign  hash_code_out = hash_code;
// fsm
	reg  [2:0] create_update_fsm;
	localparam 	IDLE 					= 3'b000 ;
	localparam	REGISTER_STATE 	= 3'b001; 
	localparam	READ_STATE 			= 3'b010; 
	localparam	FLOW_LOOKUP_STATE = 3'b011;
	localparam	CREATE_FLOW 		= 3'b100;
	localparam	UPDATE_FLOW 		= 3'b101; 
	localparam	COLLISION_PRESENT	= 3'b110;
// main
always @(posedge ACLK or negedge ARESETN )
	begin
					
		if ( !ARESETN) 
			begin
			create_update_fsm <= IDLE;
			ena <=0; 
			wea <=0;
			collision_counter_int <= 0;
			export_now <= 1'b0 ;
			end
		else	
		case (create_update_fsm)
		IDLE:	begin
					frame_five_tuple_reg <= frame_five_tuple;
					frame_ip_total_length <= pkt_info[15 : 0];
					frame_timestamp <= pkt_info[47 : 16];
					frame_tcp_flags <= pkt_info[55 : 48];
					if (tuple_and_info_valid == 1'b1) 
						begin
						ena <= 1'b1;
						create_update_fsm <= READ_STATE;
						end
					if(ena == 1'b1 && wea == 1'b1)
						begin
						ena <= 1'b0;
						wea <= 1'b0;
						end
					end
		READ_STATE: begin						
						create_update_fsm <=REGISTER_STATE ;
						ena <= 1'b0;
						end
		REGISTER_STATE: begin								
						reg_doa <= doa;		
						create_update_fsm <=FLOW_LOOKUP_STATE ;
						end
		FLOW_LOOKUP_STATE:	begin
										
						flow_byte_counter <= reg_doa[31:0] + frame_ip_total_length;
						flow_pkt_counter <= reg_doa[63 :32]  + 1'b1;
						flow_initial_timestamp <= reg_doa[127 : 96];
						flow_tcp_flags <= reg_doa[135: 128] | frame_tcp_flags;
						flow_5_tuple = reg_doa[239 : 136];	// <= khong dc xai < 
						if (ena ==1'b1 && wea == 1'b1)
						begin 
						ena <= 1'b0;
						wea <= 1'b0;
						end		
						if (reg_doa[240] == 1'b1) begin 									
							if (flow_5_tuple != frame_five_tuple_reg ) 
								create_update_fsm <= COLLISION_PRESENT; 
							else	
								create_update_fsm <= UPDATE_FLOW ;										
							end
						else 										
							create_update_fsm <= CREATE_FLOW ; 	
						begin  
						protocol <= frame_five_tuple_reg[7 : 0];
						fin_rst_flags <= (frame_tcp_flags[0] | frame_tcp_flags[2]);
						end
						end
		UPDATE_FLOW:	begin
						ena <= 1'b1;
						wea <= 1'b1;
						dia <= {1'b1,frame_five_tuple_reg, flow_tcp_flags,flow_initial_timestamp,frame_timestamp,flow_pkt_counter,flow_byte_counter};
						if ((protocol == 8'b110) && (fin_rst_flags == 6'b1))
							begin
							export_now <= 1'b1;
							export_this <= hash_code;
							end
						create_update_fsm <= IDLE ;	
						end
		CREATE_FLOW:	begin			                                
						ena <= 1'b1;
						wea <= 1'b1;
						dia <= {1'b1, frame_five_tuple_reg , frame_tcp_flags , frame_timestamp , frame_timestamp  ,flow_pkt_counter, {16'b0 , frame_ip_total_length}}; // 32'b1 # flow_pkt_counter
						if ((protocol == 8'b110) && (fin_rst_flags == 6'b1))
							begin
							export_now <= 1'b1;
							export_this <= hash_code;
							end
						create_update_fsm <= IDLE ;				
						end
		COLLISION_PRESENT :	begin
						collision_counter_int <= collision_counter_int + 1'b1;			
						create_update_fsm <= IDLE;
						end
		default:		begin 
						create_update_fsm <= IDLE ;
						end							
		endcase
			
		begin
		if( flow_exported_ok == 1'b1)
			export_now <= 1'b0;			
		end
			
end

		hash_function hash_function (
		.hash_input(frame_five_tuple_reg), 
		.hash_output(hash_code));	
		
endmodule


