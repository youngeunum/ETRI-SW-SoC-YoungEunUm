module ctrl #(parameter Xs=32, Ws=5, stride=1 )
	(input iCLK, iRSTn,
	 input iValid,
	 output reg oValid);

	reg [4:0] i, j;
	reg [4:0] i_next, j_next;
	reg iValid_dff;

	always @(posedge iCLK, negedge iRSTn) begin
		if(!iRSTn) begin
			iValid_dff<=0;
		end
		else begin
			iValid_dff<=iValid;
		end
	end
 
	always @(posedge iCLK, negedge iRSTn) begin //column
		if(!iRSTn) begin
			j<=0;
		end
		else if(j==Xs-1) begin
			j<=0;
		end
		else if(iValid_dff==1) begin
			j<=j+1;
		end
	end

	always @(posedge iCLK, negedge iRSTn) begin //row
		if(!iRSTn) begin
			i<=0;
		end
		else if((i==Xs-1)&&(j==Xs-1)) begin
			i<=0;
		end
		else if (j==Xs-1)begin
			i<=i+1;
		end
	end

	always @(posedge iCLK, negedge iRSTn) begin //column_next
		if(!iRSTn) begin
			j_next<=Ws-1;
		end
		else if ((i==i_next)&&(j==j_next)&&(j==Xs-1)) begin
			j_next<=Ws-1;
		end
		else if((i==i_next)&&(j==j_next))	begin	
			j_next<=j_next+stride;
		end
	end
		
	always @(posedge iCLK, negedge iRSTn) begin //row_next
		if(!iRSTn) begin
			i_next<=Ws-1;
		end
		else if ((i==i_next)&&(j==j_next)&&(i==Xs-1)&&(j==Xs-1)) begin
			i_next<=Ws-1;
		end
		else if((i==i_next)&&(j==j_next)&&(j==Xs-1)) begin		
			i_next<=i_next+stride;
		end
	end

	always @(posedge iCLK, negedge iRSTn) begin
		if(!iRSTn) begin
			oValid<=0;
		end
		else if((i==i_next)&&(j==j_next)&&(iValid_dff)) begin
			oValid<=1;
		end
		else begin
			oValid<=0;
		end
	end

endmodule
