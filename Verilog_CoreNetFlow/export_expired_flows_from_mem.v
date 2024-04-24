`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:41:59 03/22/2021 
// Design Name: 
// Module Name:    export_expired_flows_from_mem 
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
module export_expired_flows_from_mem(			ACLK,
												ARESETN,
												active_timeout, 
												inactive_timeout, 
												dob, 
												export_now, 
												export_this, 
												timestamp_counter, 
												fifo_full_exp, 
												addrb, 
												enb, 
												web, 
												dib, 
												flow_exported_ok, 
												fifo_exp_rst, 
												fifo_w_exp_en, 
												fifo_in_exp
    );
input 						ACLK;
input 						ARESETN;
input 			[31:0] 		active_timeout;
input 			[31:0] 		inactive_timeout;
output reg 					enb;
output reg 					web;
output reg 		[11:0] 		addrb;
input 			[240:0] 	dob; 
output reg 		[240:0] 	dib;
input wire 					export_now;
input 			[11:0]		export_this; 
input 			[31:0] 		timestamp_counter;
output reg 					flow_exported_ok;
output reg 					fifo_exp_rst;
output reg 					fifo_w_exp_en;
output reg 		[239:0]		fifo_in_exp;
input 						fifo_full_exp; 





reg 			[11:0] 		linear_counter;
reg 			[240:0] 	reg_dob;
reg 			[7:0] 		protocol;
reg 						rst_fin_flags;
reg 			[31:0] 		initial_timestamp;
reg 			[31:0] 		last_timestamp;
reg 						export_immediately;
reg 			[2:0] 		export_flows_fsm;


localparam s0 = 3'b000;
localparam READ_STATE = 3'b001;
localparam REGISTER_STATE = 3'b010;
localparam WR_ON_FIFO = 3'b011;
localparam CHK_CONDITION_STATE = 3'b100;
begin
	always @(posedge ACLK or negedge ARESETN)  begin
		if (!ARESETN) begin
					linear_counter <= 12'b0;
					enb <= 1'b0;
					web <= 1'b0;
					fifo_w_exp_en <= 1'b0;
					fifo_exp_rst <= 1'b0;
					export_flows_fsm <= s0;
				end
			else begin
				case (export_flows_fsm)
					s0 : begin
							enb <= 1'b0;
							web <= 1'b0;
							fifo_w_exp_en <= 1'b0;
						if (export_now) begin
								flow_exported_ok <= 1'b1;
								addrb[11:0] <= export_this[11:0];
								export_immediately <= 1'b1;
							end
						else	begin
								addrb[11:0] <= linear_counter[11:0];
							end
								export_flows_fsm <= READ_STATE;
						end
					READ_STATE : begin
						flow_exported_ok <= 1'b0;
						enb <= 1'b1;
						export_flows_fsm <= REGISTER_STATE;
						end
					REGISTER_STATE : begin
						enb <= 1'b0;
						reg_dob[240:0] <= dob[240:0];
						if (export_immediately) begin
							export_flows_fsm <= WR_ON_FIFO;
							end
						else	begin
							export_flows_fsm <= CHK_CONDITION_STATE;
							end
						end

					WR_ON_FIFO : begin
						fifo_in_exp[238:0] <= reg_dob[238:0];
						if (fifo_full_exp == 1'b0) begin
							fifo_w_exp_en <= 1'b1;
							enb <= 1'b1;
							web <= 1'b1;
							dib[240:0] <= 241'b0;
							export_immediately <= 1'b0;
							export_flows_fsm <= s0;
							end
						end
					CHK_CONDITION_STATE : begin
						protocol[7:0] = reg_dob[143:136]; 
						rst_fin_flags = reg_dob[128] | reg_dob[130]; 
						initial_timestamp[31:0] = reg_dob[127:96];
						last_timestamp[31:0] = reg_dob [95:64]; 
						if (reg_dob[240] == 1'b1) begin
								if ((timestamp_counter - initial_timestamp) >= active_timeout) begin
									export_flows_fsm <= WR_ON_FIFO;
									end
								else if ((timestamp_counter - last_timestamp) >= inactive_timeout) begin
									export_flows_fsm <= WR_ON_FIFO;
									end
								else begin
									linear_counter <= linear_counter + 1'b1;
									export_flows_fsm <= s0;
									end
							end
						 else begin
							linear_counter <= linear_counter + 1'b1;
							export_flows_fsm <= s0;
							end
						end
				endcase
			end
	end
end
endmodule
