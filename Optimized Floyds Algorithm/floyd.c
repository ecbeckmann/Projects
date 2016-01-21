//Authors: Eric Beckmann and Chia Ming An
//This code is written using CUDA and optimizes Floyd's algorithm for running on a GPU
//This optimization is 30 times faster than the brute force approach.

__global__ void
compute_k_iter(int *g_idata, int k, int N)
{
	// Allocate space for k column nodes in shared memory
	__shared__ int to_k[BLOCK_SIZE];
	int from_k;
	int column = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * BLOCK_SIZE + threadIdx.y;

	int i0 = row * N + column;
	int i1 = row * N + k;
	int i2 = k * N + column;
	
	int i;

	int sum;

	int factor = N%BLOCK_SIZE;
	if(factor == 0) factor = BLOCK_SIZE;

	int run_range = (N <= BLOCK_SIZE) ? 1 : N - factor + 1;
	int loop_count = min(BLOCK_SIZE, N-row);
	
// Collaboratively load k column nodes into shared memory	
to_k[threadIdx.x] = g_idata[i1 + N*threadIdx.x];

// Wait till all threads finished loading nodes
	__syncthreads();

	if ((row < run_range) && (column < N)) {
		from_k = g_idata[i2];
		for(i = 0; i < loop_count; i++){
			if(to_k[i] != -1 && from_k != -1) { 
				sum =  (to_k[i] + from_k);
				if (g_idata[i0] == -1 || sum < g_idata[i0])
					g_idata[i0] = sum;
			}
			i0 += N;
		}
	}


////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char **argv)
{
//GPU initialization as well as sequential result computed up here
////……………………..
/////……………………………….
/////////………………….
////////////
////////////////
	
	// Compute GPU parallel result
	// allocate host memory
    int mem_size = sizeof(int) * N * N;
	int *h_idata = (int *) malloc(mem_size);
	
	// initalize the memory
	memcpy(h_idata, mat, sizeof(int)*N*N);
		
    // allocate device memory
    int *d_idata;
    checkCudaErrors(cudaMalloc((void **) &d_idata, mem_size));
	
    // copy host memory to device
    checkCudaErrors(cudaMemcpy(d_idata, h_idata, mem_size,
                               cudaMemcpyHostToDevice));

	int block_size = 128;
	
	// Start a 2D thread block
	dim3 dimGrid(ceil(N/(float)block_size) , ceil(N/(float)block_size), 1);
	dim3 dimBlock(block_size,1, 1);
	
	for(int k = 0; k < N; k++) {
		// execute the kernel
		compute_k_iter<<< dimGrid, dimBlock >>>(d_idata, k, N, block_size);
	}
	
    // check if kernel execution generated and error
    getLastCudaError("Kernel execution failed");

    // copy result from device to host
    checkCudaErrors(cudaMemcpy(h_idata, d_idata, mem_size,
                               cudaMemcpyDeviceToHost));
		
	
    // cleanup memory
    checkCudaErrors(cudaFree(d_idata));
	
	//Down here:
	//compare your result with reference result
	//also do clean up for GPU
	///////////////////////
	//////////////////////////
	//////////////////////////
	/////////////////////////
}
