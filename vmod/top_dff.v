module top_dff #(parameter N=32,D=27)
	( input iCLK,iRSTn,
	  input [N-1:0] iD,
	  output [N-1:0] oQ);

	wire [N-1:0] data_arr[0:D];

	assign data_arr[0]=iD;

	genvar i;

	generate for(i=0;i<D;i=i+1) begin: gen_dff
		dff #(N) U0 (iCLK,iRSTn,data_arr[i],data_arr[i+1]);
	end
	endgenerate

	assign oQ=data_arr[D];

endmodule

