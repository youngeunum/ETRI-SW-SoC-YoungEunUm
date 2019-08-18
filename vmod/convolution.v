//all stride
module convolution #(parameter stride=1)
	(input iCLK,iRSTn,
	input signed [31:0] iX,iW,
	input [9:0] iADDR,
	input iWren,
	input iValid,
	output signed [31:0] oY,
	output oValid);

	wire signed [31:0] iX_dff;
	wire signed [31:0] O_iPsum;
	wire signed [31:0] oPsum;
	wire signed [31:0] oX;
	wire signed [31:0] w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,w21,w22,w23,w24,w25;

	dff_in						U_dff_in_top_conv		(iCLK,iRSTn,iX,iX_dff);
	weight_reg					U_weight_reg_top_conv	(iCLK,iRSTn,iWren,iADDR,iW,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,w21,w22,w23,w24,w25);
	top_stage2					U_top_stage2_top_conv	(iCLK,iRSTn,iX_dff,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,0,O_iPsum);
	top_PE						U_top_PE_top_conv		(iCLK,iRSTn,iX_dff,w21,w22,w23,w24,w25,O_iPsum,oPsum);
	Sat							U_Sat_top_conv			(oPsum,oY);
	ctrl	#(32, 5, stride)	U_ctrl_top_conv			(iCLK, iRSTn, iValid, oValid);
endmodule

