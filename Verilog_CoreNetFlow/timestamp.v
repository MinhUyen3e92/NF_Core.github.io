`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:34:10 03/12/2021 
// Design Name: 
// Module Name:    timestamp 
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
module timestamp_counter_mod( ACLK,        
							  ARESETN,
							  timestamp_counter_out);
//Input
input ACLK;
input ARESETN;
//Output
output [31:0] timestamp_counter_out;

parameter  SIM_ONLY = 0;
parameter  ACLK_FREQ = 200000000;
localparam ONE_KHZ_MAX_COUNT = ACLK_FREQ/1000;   

reg  [31:0] timestamp_counter;
reg  [31:0] divisor_for_miliseconds;
wire [31:0] max_count;  
begin
	if(SIM_ONLY == 0) 
		begin
			assign max_count = ONE_KHZ_MAX_COUNT;
		end
	else 
			assign max_count = (ONE_KHZ_MAX_COUNT/10000);
			assign timestamp_counter_out = divisor_for_miliseconds;

//assign max_count =  (SIM_ONLY == 0) ? ONE_KHZ_MAX_COUNT : (ONE_KHZ_MAX_COUNT/10000);
//assign timestamp_counter_out = divisor_for_miliseconds;

always @(posedge ACLK or negedge ARESETN) begin
		if (!ARESETN) 
				begin
					timestamp_counter <= 0;
					divisor_for_miliseconds <= 0;
				end
		else 	if(divisor_for_miliseconds == max_count) 
				begin	
					timestamp_counter <= timestamp_counter + 1;
					divisor_for_miliseconds <= 0;
				end
		else
					divisor_for_miliseconds <= divisor_for_miliseconds + 1;	
		end
	end
endmodule 