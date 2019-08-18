module PE #(parameter BW1=16, BW2=17)
	(input iCLK, iRSTn,
	input signed [31:0] iX,iW,
	input signed [BW1-1:0] iPsum,
	output reg signed [BW2-1:0] oPsum);

	always @(posedge iCLK, negedge iRSTn) begin
		if(!iRSTn) begin
			oPsum<=0;
		end
		else begin
			oPsum<=(iX*iW)+iPsum;
		end
	end

endmodule

