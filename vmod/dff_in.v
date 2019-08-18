module dff_in
	(input iCLK, iRSTn,
	input [31:0] iX,
	output reg [31:0] oX);

	always @(posedge iCLK, negedge iRSTn) begin
		if(!iRSTn) begin
			oX<=0;
		end
		else begin
			oX<=iX;
		end
	end

endmodule

