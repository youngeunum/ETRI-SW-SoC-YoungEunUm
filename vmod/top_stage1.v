module top_stage1
	(input iCLK,iRSTn,
	input signed [31:0] iX,w1,w2,w3,w4,w5,
	input signed [31:0] iPsum,
	output signed [31:0] O_iPsum);

	wire signed [31:0] oPsum_A;
	wire signed [31:0] B_iD;

	top_PE	U_top_PE_stage1		(iCLK,iRSTn,iX,w1,w2,w3,w4,w5,iPsum,oPsum_A);
	Sat		U_Sat_stage1		(oPsum_A,B_iD);
	top_dff U_top_dff_stage1	(iCLK,iRSTn,B_iD,O_iPsum);

endmodule

