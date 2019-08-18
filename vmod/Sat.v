module Sat
	(input signed [31:0] A,
	output reg signed [31:0] B);

	always @(*) begin
		if(A>32767) begin
			B=32767;
		end
		else if(A<-32768)	begin
			B=-32768;
		end
		else begin
			B=A[15:0];
		end
	end

endmodule

