`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/08/17 16:56:29
// Design Name: 
// Module Name: cnnip_ctrlr
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module cnnip_ctrlr
(
      // clock and reset signals from domain a
      input wire clk_a,
      input wire arstz_aq,
    
      // internal memories
      cnnip_mem_if.master to_input_mem,
      cnnip_mem_if.master to_weight_mem,
      cnnip_mem_if.master to_feature_mem,
     
      // configuration registers
      input wire         CMD_START,  
      input wire   [7:0] MODE_KERNEL_SIZE,
      input wire   [7:0] MODE_KERNEL_NUMS,
      input wire   [1:0] MODE_STRIDE,
      input wire         MODE_PADDING,
      output wire        CMD_DONE,
      output wire        CMD_DONE_VALID
    );
    
        wire [31:0] iX,iW;
        wire [9:0] iADDR;
        wire iWren;
        wire iValid;
        wire [31:0] oY;
        wire oValid;
        
        //모듈 호출
        convolution #(.stride(1)) u_convolution
            (.iCLK(clk_a),
            .iRSTn(arstz_aq),
            .iX(iX),
            .iW(iW),
            .iADDR(iADDR),
            .iWren(iWren),
            .iValid(iValid),
            .oY(oY),
            .oValid(oValid));
        
      // sample FSM
      enum { IDLE, Weight_Read, CONV } state_aq, state_next;
    
      // internal registers
      reg [4:0] weight_cnt;
      reg [9:0] input_cnt;
      reg [9:0] weight_addr_cnt;
      reg [9:0] input_addr_cnt;
      reg [9:0] feature_addr_cnt;
      
      // use wires as shortcuts
      wire weight_cnt_done;
      wire input_cnt_done; 
      
      assign weight_cnt_done = (weight_cnt == 5'd24);
      assign input_cnt_done = (input_cnt == 10'd1023); 
    
      always @(posedge clk_a, negedge arstz_aq)
          if (!arstz_aq) weight_cnt <= 5'b0;
          else if (state_aq == Weight_Read) weight_cnt <= weight_cnt + 1'b1;
          else weight_cnt <= 5'b0;
            
      always @(posedge clk_a, negedge arstz_aq)
          if (!arstz_aq) input_cnt <= 10'b0;
          else if (state_aq == CONV) input_cnt <= input_cnt + 1'b1;
          else input_cnt <= 10'b0;
          
      always @(posedge clk_a, negedge arstz_aq)
         if (!arstz_aq) weight_addr_cnt <= 10'b0;
         else if (state_aq == Weight_Read) weight_addr_cnt <= weight_addr_cnt + 4;
         else weight_addr_cnt <= 10'b0;         
         
      always @(posedge clk_a, negedge arstz_aq)
         if (!arstz_aq) input_addr_cnt <= 10'b0;
         else if (state_aq == CONV) input_addr_cnt <= input_addr_cnt + 4;
         else input_addr_cnt <= 10'b0;
         
      always @(posedge clk_a, negedge arstz_aq)
         if (!arstz_aq) feature_addr_cnt <= 10'b0;
         else if (state_aq == CONV && oValid==1) feature_addr_cnt <= feature_addr_cnt + 4;
         else feature_addr_cnt <= 10'b0;
    
      // state transition
      always @(posedge clk_a, negedge arstz_aq)
        if (arstz_aq == 1'b0) state_aq <= IDLE;
        else state_aq <= state_next;
    
      // state transition condition
      always @(*) begin
          state_next = state_aq;
          case (state_aq)
              IDLE : if (CMD_START) state_next = Weight_Read;
    
              Weight_Read : if (weight_cnt_done) state_next = CONV;
                
              CONV : if (CMD_DONE) state_next = IDLE;
          endcase
      end
      
      //호출한 모듈 연결하기
      assign iX = to_input_mem.dout;
      assign iW = to_weight_mem.dout;
      assign iADDR = weight_cnt;
      assign iWren = (state_aq == Weight_Read);
      assign iValid = (state_aq == CONV);
   
        //conv 연산 끝났을 때의 신호
        wire conv_done;
        //assign conv_done = ???????
    
      // output signals
      assign CMD_DONE = (state_aq == CONV) && (input_cnt_done) && (conv_done);
      assign CMD_DONE_VALID = (state_aq == CONV) &&(input_cnt_done) && (conv_done);
      
      // control signals
      assign to_input_mem.en   = (state_aq == CONV);
      assign to_input_mem.we   = 0;
      assign to_input_mem.addr = input_addr_cnt;
      assign to_input_mem.din  = 0;
    
      assign to_weight_mem.en   = (state_aq == Weight_Read);
      assign to_weight_mem.we   = 0;
      assign to_weight_mem.addr = weight_addr_cnt;
      assign to_weight_mem.din  = 0;
    
      assign to_feature_mem.en   = (state_aq == CONV);
      assign to_feature_mem.we   = oValid;
      assign to_feature_mem.addr = feature_addr_cnt;
      assign to_feature_mem.din  = oY; //나중에 내용 추가
    
endmodule // cnnip_ctrlr