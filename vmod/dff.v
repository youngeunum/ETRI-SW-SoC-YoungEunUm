module dff #(parameter N=32)
	(input iCLK,iRSTn,
	input [N-1:0] iD,
	output reg [N-1:0] oQ);

	always @(posedge iCLK, negedge iRSTn) begin
		if(!iRSTn) begin
			oQ<=0;
		end
		else begin
			oQ<=iD;
		end
	end

endmodule

