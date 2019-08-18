module top_PE 
	(input iCLK, iRSTn,
	input signed [31:0] iX,
	input signed [31:0] w1,w2,w3,w4,w5,
	input signed [31:0] iPsum,
	output signed [31:0] oPsum);

	wire signed [31:0] oPsum1,oPsum2;
	wire signed [31:0] oPsum3,oPsum4;

	PE #(32,32) U_PE_top_PE_1	(iCLK,iRSTn,iX,w1,iPsum,oPsum1);
	PE #(32,32) U_PE_top_PE_2	(iCLK,iRSTn,iX,w2,oPsum1,oPsum2);
	PE #(32,32) U_PE_top_PE_3	(iCLK,iRSTn,iX,w3,oPsum2,oPsum3);
	PE #(32,32) U_PE_top_PE_4	(iCLK,iRSTn,iX,w4,oPsum3,oPsum4);
	PE #(32,32) U_PE_top_PE_5	(iCLK,iRSTn,iX,w5,oPsum4,oPsum);

endmodule

