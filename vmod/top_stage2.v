module top_stage2
	(input iCLK,iRSTn,
	input signed [31:0] iX,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,
	input signed [31:0] iPsum,
	output signed [31:0] O_iPsum);

	wire signed [31:0] O_iPsum1,O_iPsum2,O_iPsum3;

	top_stage1 U_stage1_stage2_1	(iCLK,iRSTn,iX,w1,w2,w3,w4,w5,iPsum,O_iPsum1);
	top_stage1 U_stage1_stage2_2	(iCLK,iRSTn,iX,w6,w7,w8,w9,w10,O_iPsum1,O_iPsum2);
	top_stage1 U_stage1_stage2_3	(iCLK,iRSTn,iX,w11,w12,w13,w14,w15,O_iPsum2,O_iPsum3);
	top_stage1 U_stage1_stage2_4	(iCLK,iRSTn,iX,w16,w17,w18,w19,w20,O_iPsum3,O_iPsum);

endmodule

