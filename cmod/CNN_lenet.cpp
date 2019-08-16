    #include <opencv2/opencv.hpp>
    #include <iostream>
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    using namespace std;
    using namespace cv;

    void fully_connected(int input_size, double *input, int output_size, double *output, double **weight, double *bias);
    double Activation(double *input, int input_size);
    double softmax(double *input, int input_size);
    
    int main()
    {
        int i;
        int j;
        int n;
        int f;
        int fi;
        int fj;
        int ni;
        int nj;
        double max;
        double sum;
        
        int ch_size;
        int size_r;
        int size_c;
        int size;
        int filter_size;
        int input_size;
        int output_size;
        int stride;
        int nfilters;
        int out_size;
        int output_size_r;
        int output_size_c;
        int num;

        double ***input;
        double ***conv1;
        double ***max_pooling1;
        double ***conv2;
        double ***max_pooling2;
        double *change;
        double *fully1;
        double *fully2;

        double ****conv1_weight;
        double ****conv2_weight;
        double **fully1_weight;
        double **fully2_weight;
        double *bias1;
        double *bias2;
        double *bias3;
        double *bias4;
        
        FILE *fp1;
        FILE *fp2;
        FILE *fp3;
        FILE *fp4;
        FILE *fp5;
        FILE *fp6;
        FILE *fp7;
        FILE *fp8;

        fp1 = fopen("./conv1_weight.txt","r");
        fp2 = fopen("./conv2_weight.txt","r");
        fp3 = fopen("./fully1_weight.txt","r");
        fp4 = fopen("./fully2_weight.txt","r");
        fp5 = fopen("./conv1_bias.txt","r");
        fp6 = fopen("./conv2_bias.txt","r");
        fp7 = fopen("./fully1_bias.txt","r");
        fp8 = fopen("./fully2_bias.txt","r");

//---------------------------------------------------------------------------------------------------------------
        //동적 메모리 할당(input)
        size_r = 28;
        size_c = 28;
        ch_size = 1;
        input = (double ***)malloc(ch_size*sizeof(double**));
        for(i=0; i<ch_size; i++){
            *(input+i) = (double**)malloc(size_r*sizeof(double*));
            for(j=0; j<size_r; j++){
                *(*(input+i)+j) = (double*)malloc(size_c*sizeof(double));
            }
        }

        //동적 메모리 할당(conv1)
        size_r = 24;
        size_c = 24;
        ch_size = 20;
        conv1 = (double ***)malloc(ch_size*sizeof(double**));
        for(i=0; i<ch_size; i++){
            *(conv1+i) = (double**)malloc(size_r*sizeof(double*));
            for(j=0; j<size_r; j++){
                *(*(conv1+i)+j) = (double*)malloc(size_c*sizeof(double));
            }
        }

        //동적 메모리 할당(max_pooling1)
        size_r = 12;
        size_c = 12;
        ch_size = 20;
        max_pooling1 = (double ***)malloc(ch_size*sizeof(double**));
        for(i=0; i<ch_size; i++){
            *(max_pooling1+i) = (double**)malloc(size_r*sizeof(double*));
            for(j=0; j<size_r; j++){
                *(*(max_pooling1+i)+j) = (double*)malloc(size_c*sizeof(double));
            }
        }

        //동적 메모리 할당(conv2)
        size_r = 8;
        size_c = 8;
        ch_size = 50;
        conv2 = (double ***)malloc(ch_size*sizeof(double**));
        for(i=0; i<ch_size; i++){
            *(conv2+i) = (double**)malloc(size_r*sizeof(double*));
            for(j=0; j<size_r; j++){
                *(*(conv2+i)+j) = (double*)malloc(size_c*sizeof(double));
            }
        }

        //동적 메모리 할당(max_pooling2)
        size_r = 4;
        size_c = 4;
        ch_size = 50;
        max_pooling2 = (double ***)malloc(ch_size*sizeof(double**));
        for(i=0; i<ch_size; i++){
            *(max_pooling2+i) = (double**)malloc(size_r*sizeof(double*));
            for(j=0; j<size_r; j++){
                *(*(max_pooling2+i)+j) = (double*)malloc(size_c*sizeof(double));
            }
        }

        //동적 메모리 할당(change)
        size = 800;
        change = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당(fully1)
        size = 500;
        fully1 = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당(fully2)
        size = 10;
        fully2 = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당(conv1_weight)
        ch_size = 1;
        filter_size = 5;
        num = 20;
        conv1_weight = (double ****)malloc(num*sizeof(double***));
        for(i=0; i<num; i++){
            *(conv1_weight+i) = (double***)malloc(ch_size*sizeof(double**));
            for(j=0; j<ch_size; j++){
                *(*(conv1_weight+i)+j) = (double**)malloc(filter_size*sizeof(double*));
                for(n=0; n<filter_size; n++){
                    *(*(*(conv1_weight+i)+j)+n) = (double*)malloc(filter_size*sizeof(double));
                }
            }
        }

        //동적 메모리 할당(conv2_weight)
        ch_size = 20;
        filter_size = 5;
        num = 50;
        conv2_weight = (double ****)malloc(num*sizeof(double***));
        for(i=0; i<num; i++){
            *(conv2_weight+i) = (double***)malloc(ch_size*sizeof(double**));
            for(j=0; j<ch_size; j++){
                *(*(conv2_weight+i)+j) = (double**)malloc(filter_size*sizeof(double*));
                for(n=0; n<filter_size; n++){
                    *(*(*(conv2_weight+i)+j)+n) = (double*)malloc(filter_size*sizeof(double));
                }
            }
        }

        //동적 메모리 할당(fully1_weight)
        input_size = 800;
        output_size = 500;
        fully1_weight = (double **)malloc(output_size*sizeof(double*));
        for(i=0; i<output_size; i++)
        {
            *(fully1_weight + i) = (double *)malloc(input_size*sizeof(double));
        }		

        //동적 메모리 할당(fully2_weight)
        input_size = 500;
        output_size = 10;
        fully2_weight = (double **)malloc(output_size*sizeof(double*));
        for(i=0; i<output_size; i++)
        {
            *(fully2_weight + i) = (double *)malloc(input_size*sizeof(double));
        }

        //동적 메모리 할당 (bias1)
        size = 20;
        bias1 = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당 (bias2)
        size = 50;
        bias2 = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당 (bias3)
        size = 500;
        bias3 = (double *)malloc(size*sizeof(double));

        //동적 메모리 할당 (bias4)
        size = 10;
        bias4 = (double *)malloc(size*sizeof(double));
        
//---------------------------------------------------------------------------------------------------------------

        //conv1_weight
        ch_size = 20;
        filter_size = 5;
        nfilters = 1;
        for(i=0; i<ch_size; i++){
            for(j=0; j<nfilters; j++){
                for(n=0; n<filter_size; n++){
                    for(f=0; f<filter_size; f++){
                        fscanf(fp1, " %lf ,", (*(*(*(conv1_weight+i)+j)+n)+f));					
                    }
                }
            }
        }

        //conv2_weight
        ch_size = 50;
        filter_size = 5;
        nfilters = 20;
        for(i=0; i<ch_size; i++){
            for(j=0; j<nfilters; j++){
                for(n=0; n<filter_size; n++){
                    for(f=0; f<filter_size; f++){
                        fscanf(fp2, " %lf ,", (*(*(*(conv2_weight+i)+j)+n)+f));					
                    }
                }
            }
        }

        //fully1_weight
        input_size = 800;
        output_size = 500;
        for(i=0; i<output_size; i++){
            for(j=0; j<input_size; j++){
                fscanf(fp3, " %lf ,", (*(fully1_weight+i)+j));					
            }
        }

        //fully2_weight
        input_size = 500;
        output_size = 10;
        for(i=0; i<output_size; i++){
            for(j=0; j<input_size; j++){
                fscanf(fp4, " %lf ,", (*(fully2_weight+i)+j));					
            }
        }

        //bias1
        size = 20;
        for(i=0; i<size; i++){
            fscanf(fp5, " %lf ,", bias1+i);
        }

        //bias2
        size = 50;
        for(i=0; i<size; i++){
            fscanf(fp6, " %lf ,", bias2+i);
        }

        //bias3
        size = 500;
        for(i=0; i<size; i++){
            fscanf(fp7, " %lf ,", bias3+i);
        }

        //bias4
        size = 10;
        for(i=0; i<size; i++){
            fscanf(fp8, " %lf ,", bias4+i);
        }

//---------------------------------------------------------------------------------------------------------------

        //이미지 받기
        Mat image;
        image = imread("99.png",IMREAD_GRAYSCALE);
        if(image.empty())
        {
            cout<<"could not open or find the image" <<endl;
            return -1;
        }

        //image를 input에 넣기
        ch_size = 1;
        for(i=0; i<ch_size; i++){
            for(j=0; j<image.rows; j++){
                for(n=0; n<image.cols; n++){
                    input[i][j][n] = image.at<uchar>(j,n);
                }
            }
        }
//---------------------------------------------------------------------------------------------------------------

        //conv1
        ch_size = 20;
        output_size_r = 24;
        output_size_c = 24;
        nfilters = 1;
        filter_size = 5;
        stride = 1;
        for(i=0; i<ch_size; i++){
            for(j=0; j<output_size_r; j++){
                for(n=0; n<output_size_c; n++){
                    for(fi=0; fi<nfilters; fi++){
                        for(fj=0; fj<filter_size; fj++){
                            for(ni=0; ni<filter_size; ni++){
                                conv1[i][j][n] += input[fi][stride*j+fj][stride*n+ni] * conv1_weight[i][fi][fj][ni];
                            }			
                        }
                    }
                    conv1[i][j][n] +=  bias1[i];
                }
            }
        }

        //max_pooling1
        ch_size = 20;
        output_size_r = 12;
        output_size_c = 12;
        nfilters = 1;
        filter_size = 2;
        stride = 2;
        max = INT_MIN;
        for(i=0; i<ch_size; i++){
            for(j=0; j<output_size_r; j++){
                for(n=0; n<output_size_c; n++){
                    //for(fi=0; fi<nfilters; fi++){
                        for(fj=0; fj<filter_size; fj++){
                            for(ni=0; ni<filter_size; ni++){
                                if(max<conv1[i][stride*j+fj][stride*n+ni])
                                    max = conv1[i][stride*j+fj][stride*n+ni];
                            }
                        }
                    //}
                    max_pooling1[i][j][n] = max;
                    max = INT_MIN;
                }
            }
        }

        //conv2
        ch_size = 50;
        output_size_r = 8;
        output_size_c = 8;
        nfilters = 20;
        filter_size = 5;
        stride = 1;
        for(i=0; i<ch_size; i++){
            for(j=0; j<output_size_r; j++){
                for(n=0; n<output_size_c; n++){
                    for(fi=0; fi<nfilters; fi++){
                        for(fj=0; fj<filter_size; fj++){
                            for(ni=0; ni<filter_size; ni++){
                                conv2[i][j][n] += max_pooling1[fi][stride*j+fj][stride*n+ni] * conv2_weight[i][fi][fj][ni];
                            }			
                        }
                    }
                     conv2[i][j][n] +=  bias2[i];
                }
            }
        }


        //max_pooling2
        ch_size = 50;
        output_size_r = 4;
        output_size_c = 4;
        nfilters = 1;
        filter_size = 2;
        stride = 2;
        max = INT_MIN;
        for(i=0; i<ch_size; i++){
            for(j=0; j<output_size_r; j++){
                for(n=0; n<output_size_c; n++){
                    for(fi=0; fi<nfilters; fi++){
                        for(fj=0; fj<filter_size; fj++){
                            for(ni=0; ni<filter_size; ni++){
                                if(max<conv2[i][stride*j+fj][stride*n+ni])
                                    max = conv2[i][stride*j+fj][stride*n+ni];
                            }
                        }
                    }
                    max_pooling2[i][j][n] = max;
                    max = INT_MIN;
                }
            }
        }

        //3차원을 1차원으로
        ch_size = 50;
        size_r = 4;
        size_c = 4;
        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                for(n = 0; n<size_c; n++){
                    change[(i*size_c*size_r)+(j*size_c)+n] = max_pooling2[i][j][n];
                }
            }
        }

        //fully_connected1
        input_size = 800;
        output_size = 500;
        fully_connected(input_size, change, output_size, fully1, fully1_weight, bias3);
        
        //Activation
        input_size = 500;
        Activation(fully1, input_size);
        
        //fully_connected2
        input_size = 500;
        output_size = 10;
        fully_connected(input_size, fully1, output_size, fully2, fully2_weight, bias4);

        //fc 끝난 후 출력해보기
        for(i=0; i<10; i++){
            printf("[%d] %lf \n", i , fully2[i]);
        }
        printf("\n");

        //Softmax
        input_size = 10;
        softmax(fully2, input_size);
        
        //sotfmax 후 출력해보기
        for(i=0; i<input_size; i++){
            printf("[%d] %.20lf \n", i , fully2[i]);
        }
//---------------------------------------------------------------------------------------------------------------

        //할당한 동적 메모리 free
        
        //input
        ch_size = 1;
        size_r = 28;
        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                free(*(*(input+i)+j));
            }
            free(*(input+i));
        }
        free(input);

        //conv1
        ch_size = 20;
        size_r = 24;
        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                free(*(*(conv1+i)+j));
            }
            free(*(conv1+i));
        }
        free(conv1);

        //max_pooling1
        ch_size = 20;
        size_r = 12;
        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                free(*(*(max_pooling1+i)+j));
            }
            free(*(max_pooling1+i));
        }
        free(max_pooling1);

        //conv2
        ch_size = 50;
        size_r = 8;

        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                free(*(*(conv2+i)+j));
            }
            free(*(conv2+i));
        }
        free(conv2);

        //5. max_pooing2
        ch_size = 50;
        size_r = 4;

        for(i=0; i<ch_size; i++){
            for(j=0; j<size_r; j++){
                free(*(*(max_pooling2+i)+j));
            }
            free(*(max_pooling2+i));
        }
        free(max_pooling2);

        //change
        free(change);

        //fully1
        free(fully1);

        //fully2
        free(fully2);

        //conv1_weight
        num = 20;
        ch_size = 1;
        size_r = 5;
        for(i=0; i<num; i++){
            for(j=0; j<ch_size; j++){
                for(n=0; n<size_r; n++){
                    free(*(*(*(conv1_weight+i)+j)+n));		
                }
                free(*(*(conv1_weight+i)+j));
            }
            free(*(conv1_weight+i));
        }
        free(conv1_weight);

        //conv2_weight 
        num = 50;
        ch_size = 20;
        size_r = 5;
        for(i=0; i<num; i++){
            for(j=0; j<ch_size; j++){
                for(n=0; n<size_r; n++){
                    free(*(*(*(conv2_weight+i)+j)+n));		
                }
                free(*(*(conv2_weight+i)+j));
            }
            free(*(conv2_weight+i));
        }
        free(conv2_weight);

        //fully1_weight
        out_size = 500;
        for(i=0; i<out_size; i++){
            free(*(fully1_weight+i));
        }
        free(fully1_weight);

        //fully2_weight4
        out_size = 10;
        for(i=0; i<out_size; i++){
            free(*(fully2_weight+i));
        }
        free(fully2_weight);

        //bias1
        free(bias1);

        //bias2
        free(bias2);

        //bias3
        free(bias3);

        //bias4
        free(bias4);

        return 0;
    }



double Activation(double *input, int input_size) //relu
{
    int i;

    for(i=0; i<input_size; i++){
        if(input[i]<0){ 
            input[i] = 0;
        }
        else {
            input[i] = input[i];
        }
    }
}



void fully_connected(int input_size, double *input, int output_size, double *output, double **weight, double *bias)
{
	int i;
    int j;

	for(i=0; i<output_size; i++){
		for(j=0; j<input_size; j++){
			output[i] += weight[i][j] * input[j];
		}
        output[i] += bias[i];
	}
}



double softmax(double *input, int input_size)
{
	double max = input[0];
	double sum;
	int i;

	for(i=0; i<input_size; i++){
		if(max<input[i])
		{		
			max = input[i];
		}
	}
	
	for(i=0; i<input_size; i++){
		input[i] = exp(input[i] - max);
		sum = sum + input[i];
	}

	for(i=0; i<input_size; i++){
		input[i] = input[i]/sum;
	} 
}