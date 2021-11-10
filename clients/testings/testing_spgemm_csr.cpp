/* ************************************************************************
 * Copyright (c) 2020-2021 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

#include "auto_testing_bad_arg.hpp"
#include "testing.hpp"

template <typename T, typename I = rocsparse_int, typename J = rocsparse_int>
rocsparse_status rocsparse_csr_set_pointers(rocsparse_spmat_descr       descr,
                                            device_csr_matrix<T, I, J>& csr_matrix)
{
    return rocsparse_csr_set_pointers(descr, csr_matrix.ptr, csr_matrix.ind, csr_matrix.val);
}

template <typename I, typename J, typename T>
void testing_spgemm_csr_bad_arg(const Arguments& arg)
{
    static const size_t safe_size = 100;

    // Create rocsparse handle
    rocsparse_local_handle local_handle;

    rocsparse_handle handle = local_handle;
    J                m      = safe_size;
    J                n      = safe_size;
    J                k      = safe_size;
    I                nnz_A  = safe_size;
    I                nnz_B  = safe_size;
    I                nnz_C  = safe_size;
    I                nnz_D  = safe_size;

    void* csr_row_ptr_A = (void*)0x4;
    void* csr_col_ind_A = (void*)0x4;
    void* csr_val_A     = (void*)0x4;
    void* csr_row_ptr_B = (void*)0x4;
    void* csr_col_ind_B = (void*)0x4;
    void* csr_val_B     = (void*)0x4;
    void* csr_row_ptr_C = (void*)0x4;
    void* csr_col_ind_C = (void*)0x4;
    void* csr_val_C     = (void*)0x4;
    void* csr_row_ptr_D = (void*)0x4;
    void* csr_col_ind_D = (void*)0x4;
    void* csr_val_D     = (void*)0x4;

    rocsparse_operation    trans_A = rocsparse_operation_none;
    rocsparse_operation    trans_B = rocsparse_operation_none;
    rocsparse_index_base   base    = rocsparse_index_base_zero;
    rocsparse_spgemm_alg   alg     = rocsparse_spgemm_alg_default;
    rocsparse_spgemm_stage stage   = rocsparse_spgemm_stage_auto;

    // Index and data type
    rocsparse_indextype itype = get_indextype<I>();
    rocsparse_indextype jtype = get_indextype<J>();
    rocsparse_datatype  ttype = get_datatype<T>();

    // SpGEMM structures
    rocsparse_local_spmat local_mat_A(m,
                                      k,
                                      nnz_A,
                                      csr_row_ptr_A,
                                      csr_col_ind_A,
                                      csr_val_A,
                                      itype,
                                      jtype,
                                      base,
                                      ttype,
                                      rocsparse_format_csr);
    rocsparse_local_spmat local_mat_B(k,
                                      n,
                                      nnz_B,
                                      csr_row_ptr_B,
                                      csr_col_ind_B,
                                      csr_val_B,
                                      itype,
                                      jtype,
                                      base,
                                      ttype,
                                      rocsparse_format_csr);
    rocsparse_local_spmat local_mat_C(m,
                                      n,
                                      nnz_C,
                                      csr_row_ptr_C,
                                      csr_col_ind_C,
                                      csr_val_C,
                                      itype,
                                      jtype,
                                      base,
                                      ttype,
                                      rocsparse_format_csr);
    rocsparse_local_spmat local_mat_D(m,
                                      n,
                                      nnz_D,
                                      csr_row_ptr_D,
                                      csr_col_ind_D,
                                      csr_val_D,
                                      itype,
                                      jtype,
                                      base,
                                      ttype,
                                      rocsparse_format_csr);

    rocsparse_spmat_descr mat_A = local_mat_A;
    rocsparse_spmat_descr mat_B = local_mat_B;
    rocsparse_spmat_descr mat_C = local_mat_C;
    rocsparse_spmat_descr mat_D = local_mat_D;

    int       nargs_to_exclude   = 4;
    const int args_to_exclude[4] = {3, 6, 12, 13};

    // 4 Scenarios need to be tested:

    // Scenario 1: alpha == nullptr && beta == nullptr
    // Scenario 2: alpha != nullptr && beta == nullptr
    // Scenario 3: alpha == nullptr && beta != nullptr
    // Scenario 4: alpha != nullptr && beta != nullptr

#define PARAMS                                                                            \
    handle, trans_A, trans_B, alpha, mat_A, mat_B, beta, mat_D, mat_C, ttype, alg, stage, \
        buffer_size, temp_buffer
    // ###############################################
    // Scenario 1: alpha == nullptr && beta == nullptr
    // ###############################################
    {
        const T* alpha = (const T*)nullptr;
        const T* beta  = (const T*)nullptr;

        size_t* buffer_size = (size_t*)0x4;
        void*   temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = (size_t*)0x4;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);
    }

    // ###############################################
    // Scenario 2: alpha != nullptr && beta == nullptr
    // ###############################################
    {
        const T* alpha = (const T*)0x4;
        const T* beta  = (const T*)nullptr;

        size_t* buffer_size = (size_t*)0x4;
        void*   temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = (size_t*)0x4;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);
    }

    // ###############################################
    // Scenario 3: alpha == nullptr && beta != nullptr
    // ###############################################
    {
        const T* alpha = (const T*)nullptr;
        const T* beta  = (const T*)0x4;

        size_t* buffer_size = (size_t*)0x4;
        void*   temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = (size_t*)0x4;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);
    }

    // ###############################################
    // Scenario 4: alpha != nullptr && beta != nullptr
    // ###############################################
    {
        const T* alpha = (const T*)0x4;
        const T* beta  = (const T*)0x4;

        size_t* buffer_size = (size_t*)0x4;
        void*   temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = (size_t*)0x4;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = (void*)0x4;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);

        buffer_size = nullptr;
        temp_buffer = nullptr;
        auto_testing_bad_arg(rocsparse_spgemm, nargs_to_exclude, args_to_exclude, PARAMS);
    }
#undef PARAMS

    const T* alpha = (const T*)0x4;
    const T* beta  = (const T*)0x4;
    EXPECT_ROCSPARSE_STATUS(rocsparse_spgemm(handle,
                                             trans_A,
                                             trans_B,
                                             alpha,
                                             mat_A,
                                             mat_B,
                                             beta,
                                             mat_D,
                                             mat_C,
                                             ttype,
                                             alg,
                                             stage,
                                             nullptr,
                                             nullptr),
                            rocsparse_status_invalid_pointer);
}

template <typename I, typename J, typename T>
void testing_spgemm_csr(const Arguments& arg)
{
    J                    M       = arg.M;
    J                    N       = arg.N;
    J                    K       = arg.K;
    rocsparse_operation  trans_A = arg.transA;
    rocsparse_operation  trans_B = arg.transA;
    rocsparse_index_base base_A  = arg.baseA;
    rocsparse_index_base base_B  = arg.baseB;
    rocsparse_index_base base_C  = arg.baseC;
    rocsparse_index_base base_D  = arg.baseD;
    rocsparse_spgemm_alg alg     = arg.spgemm_alg;

    T h_alpha = arg.get_alpha<T>();
    T h_beta  = arg.get_beta<T>();

    // -99 means nullptr
    T* h_alpha_ptr = (h_alpha == (T)-99) ? nullptr : &h_alpha;
    T* h_beta_ptr  = (h_beta == (T)-99) ? nullptr : &h_beta;

    // Index and data type
    rocsparse_datatype ttype = get_datatype<T>();

    // SpGEMM stage
    rocsparse_spgemm_stage stage = rocsparse_spgemm_stage_auto;

    // Create rocsparse handle
    rocsparse_local_handle handle;
    using host_csr   = host_csr_matrix<T, I, J>;
    using device_csr = device_csr_matrix<T, I, J>;

#define PARAMS(alpha_, A_, B_, D_, beta_, C_, buffer_)                                        \
    handle, trans_A, trans_B, alpha_, A_, B_, beta_, D_, C_, ttype, alg, stage, &buffer_size, \
        dbuffer

    // Argument sanity check before allocating invalid memory
    if((M <= 0 || N <= 0 || K <= 0) && (M <= 0 || N <= 0 || K != 0 || h_beta_ptr == nullptr))
    {
        static const I safe_size = 1;

        // Allocate memory on device
        // device_csr dA { safe_size,safe_size,safe_size, {}, {}, {} };

        I nnz_A = (M > 0 && K > 0) ? safe_size : 0;
        I nnz_B = (K > 0 && N > 0) ? safe_size : 0;
        I nnz_D = (M > 0 && N > 0) ? safe_size : 0;

        device_csr dA(
            std::max(M, static_cast<J>(0)), std::max(K, static_cast<J>(0)), nnz_A, base_A);
        dA.m = M; // not fancy but okay.
        dA.n = K;

        device_csr dB(
            std::max(K, static_cast<J>(0)), std::max(N, static_cast<J>(0)), nnz_B, base_B);
        dB.m = K;
        dB.n = N;

        device_csr dC(std::max(M, static_cast<J>(0)),
                      std::max(N, static_cast<J>(0)),
                      static_cast<I>(0),
                      base_C);
        dC.m = M;
        dC.n = N;

        device_csr dD(
            std::max(M, static_cast<J>(0)), std::max(N, static_cast<J>(0)), nnz_D, base_D);
        dD.m = M;
        dD.n = N;

        // Check structures
        rocsparse_local_spmat A(dA), B(dB), C(dC), D(dD);

        // Pointer mode
        CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));

        // Query SpGEMM buffer
        size_t buffer_size;
        void*  dbuffer = nullptr;
        EXPECT_ROCSPARSE_STATUS(
            rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)),
            rocsparse_status_success);

        CHECK_HIP_ERROR(hipMalloc(&dbuffer, safe_size));

        EXPECT_ROCSPARSE_STATUS(
            rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)),
            rocsparse_status_success);

        // Verify that nnz_C is equal to zero
        {
            int64_t                  rows_C;
            int64_t                  cols_C;
            int64_t                  nnz_C;
            static constexpr int64_t zero = 0;
            CHECK_ROCSPARSE_ERROR(rocsparse_spmat_get_size(C, &rows_C, &cols_C, &nnz_C));

            unit_check_scalar(zero, nnz_C);
        }

        CHECK_HIP_ERROR(hipFree(dbuffer));
        return;
    }

    // Allocate host memory for matrix

    //
    // Declare host matrices.
    //
    host_csr hA, hB, hD;
    //
    // Init matrix A from the input rocsparse_matrix_init
    //

    const bool            to_int    = arg.timing ? false : true;
    static constexpr bool full_rank = false;

    {
        rocsparse_matrix_factory<T, I, J> matrix_factory(arg, to_int, full_rank);
        matrix_factory.init_csr(hA, M, K, arg.baseA);
    }
    //
    // Init matrix B and D from rocsparse_matrix_init random.
    //
    {
        static constexpr bool             noseed = true;
        rocsparse_matrix_factory<T, I, J> matrix_factory(
            arg, rocsparse_matrix_random, to_int, full_rank, noseed);
        matrix_factory.init_csr(hB, K, N, arg.baseB);
        matrix_factory.init_csr(hD, M, N, arg.baseD);
    }

    //
    // Declare device matrices.
    //
    device_csr dA(hA);
    device_csr dB(hB);
    device_csr dD(hD);

    //
    // Declare local spmat.
    //
    rocsparse_local_spmat A(dA), B(dB), D(dD);
    if(arg.unit_check)
    {
        //
        // Compute C on host.
        //
        host_csr hC;

        {
            I hC_nnz = 0;
            hC.define(M, N, hC_nnz, base_C);
            host_csrgemm_nnz<T, I, J>(M,
                                      N,
                                      K,
                                      h_alpha_ptr,
                                      hA.ptr,
                                      hA.ind,
                                      hB.ptr,
                                      hB.ind,
                                      h_beta_ptr,
                                      hD.ptr,
                                      hD.ind,
                                      hC.ptr,
                                      &hC_nnz,
                                      hA.base,
                                      hB.base,
                                      hC.base,
                                      hD.base);
            hC.define(hC.m, hC.n, hC_nnz, hC.base);
        }

        host_csrgemm<T, I, J>(M,
                              N,
                              K,
                              h_alpha_ptr,
                              hA.ptr,
                              hA.ind,
                              hA.val,
                              hB.ptr,
                              hB.ind,
                              hB.val,
                              h_beta_ptr,
                              hD.ptr,
                              hD.ind,
                              hD.val,
                              hC.ptr,
                              hC.ind,
                              hC.val,
                              hA.base,
                              hB.base,
                              hC.base,
                              hD.base);

        //
        // Compute C on device with mode host.
        //
        {
            device_csr dC;
            dC.define(M, N, 0, base_C);
            rocsparse_local_spmat C(dC);
            CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));

            {
                size_t buffer_size;
                void*  dbuffer = nullptr;

                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
                CHECK_HIP_ERROR(hipMalloc(&dbuffer, buffer_size));

                //
                // Compute symbolic C.
                //
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));

                //
                // Update memory.
                //
                {
                    int64_t C_m, C_n, C_nnz;
                    CHECK_ROCSPARSE_ERROR(rocsparse_spmat_get_size(C, &C_m, &C_n, &C_nnz));
                    dC.define(dC.m, dC.n, C_nnz, dC.base);
                    CHECK_ROCSPARSE_ERROR(rocsparse_csr_set_pointers(C, dC));
                }

                //
                // Compute numeric C.
                //
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
                CHECK_HIP_ERROR(hipFree(dbuffer));
            }

            //
            // Check
            //
            if((!h_alpha_ptr || std::abs(*h_alpha_ptr) == ((I)std::abs(*h_alpha_ptr)))
               && (!h_beta_ptr || std::abs(*h_beta_ptr) == ((I)std::abs(*h_beta_ptr))))
            {
                hC.near_check(dC);
                // hC.unit_check(dC);
            }
            else
            {
                hC.near_check(dC);
            }

            {
                device_vector<T> d_alpha(1);
                device_vector<T> d_beta(1);
                CHECK_HIP_ERROR(hipMemcpy(d_alpha, &h_alpha, sizeof(T), hipMemcpyHostToDevice));
                CHECK_HIP_ERROR(hipMemcpy(d_beta, &h_beta, sizeof(T), hipMemcpyHostToDevice));
                T* d_alpha_ptr = (h_alpha == (T)-99) ? nullptr : d_alpha;
                T* d_beta_ptr  = (h_beta == (T)-99) ? nullptr : d_beta;

                device_csr dC;
                dC.define(M, N, 0, base_C);
                rocsparse_local_spmat C(dC);
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_device));

                {
                    size_t buffer_size;
                    void*  dbuffer = nullptr;

                    CHECK_ROCSPARSE_ERROR(
                        rocsparse_spgemm(PARAMS(d_alpha_ptr, A, B, D, d_beta_ptr, C, dbuffer)));
                    CHECK_HIP_ERROR(hipMalloc(&dbuffer, buffer_size));

                    //
                    // Compute symbolic C.
                    //
                    CHECK_ROCSPARSE_ERROR(
                        rocsparse_spgemm(PARAMS(d_alpha_ptr, A, B, D, d_beta_ptr, C, dbuffer)));

                    //
                    // Update memory.
                    //
                    {
                        int64_t C_m, C_n, C_nnz;
                        CHECK_ROCSPARSE_ERROR(rocsparse_spmat_get_size(C, &C_m, &C_n, &C_nnz));
                        dC.define(dC.m, dC.n, C_nnz, dC.base);
                        CHECK_ROCSPARSE_ERROR(rocsparse_csr_set_pointers(C, dC));
                    }

                    //
                    // Compute numeric C.
                    //
                    CHECK_ROCSPARSE_ERROR(
                        rocsparse_spgemm(PARAMS(d_alpha_ptr, A, B, D, d_beta_ptr, C, dbuffer)));
                    CHECK_HIP_ERROR(hipFree(dbuffer));
                }

                if((!h_alpha_ptr || std::abs(*h_alpha_ptr) == ((I)std::abs(*h_alpha_ptr)))
                   && (!h_beta_ptr || std::abs(*h_beta_ptr) == ((I)std::abs(*h_beta_ptr))))
                {
                    //
                    // Check
                    //
                    // hC.unit_check(dC);
                    hC.near_check(dC);
                }
                else
                {
                    hC.near_check(dC);
                }
            }
        }
    }

    if(arg.timing)
    {

        int number_cold_calls = 2;
        int number_hot_calls  = arg.iters;

        CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));

        {
            device_csr dC;
            dC.define(M, N, 0, base_C);
            //
            // Warm up
            //
            for(int iter = 0; iter < number_cold_calls; ++iter)
            {
                // Sparse matrix descriptor C
                rocsparse_local_spmat C(dC);
                // Query for buffer size
                size_t buffer_size;
                void*  dbuffer = nullptr;
                //
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
                //
                CHECK_HIP_ERROR(hipMalloc(&dbuffer, buffer_size));
                //
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
                //
                {
                    int64_t C_m, C_n, C_nnz;
                    CHECK_ROCSPARSE_ERROR(rocsparse_spmat_get_size(C, &C_m, &C_n, &C_nnz));
                    dC.define(dC.m, dC.n, C_nnz, dC.base);
                    CHECK_ROCSPARSE_ERROR(rocsparse_csr_set_pointers(C, dC));
                }
                //
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
                //
                CHECK_HIP_ERROR(hipFree(dbuffer));
            }
        }

        double gpu_analysis_time_used, gpu_solve_time_used;

        //
        // Performance run
        //
        int64_t C_nnz;

        {
            device_csr dC;
            dC.define(M, N, 0, base_C);
            rocsparse_local_spmat C(dC);

            gpu_analysis_time_used = get_time_us();

            size_t buffer_size;
            void*  dbuffer = nullptr;
            CHECK_ROCSPARSE_ERROR(
                rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
            //
            CHECK_HIP_ERROR(hipMalloc(&dbuffer, buffer_size));
            //
            CHECK_ROCSPARSE_ERROR(
                rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
            //

            gpu_analysis_time_used = get_time_us() - gpu_analysis_time_used;

            {
                int64_t C_m, C_n;
                CHECK_ROCSPARSE_ERROR(rocsparse_spmat_get_size(C, &C_m, &C_n, &C_nnz));
                dC.define(dC.m, dC.n, C_nnz, dC.base);
                CHECK_ROCSPARSE_ERROR(rocsparse_csr_set_pointers(C, dC));
            }

            gpu_solve_time_used = get_time_us();

            //
            // Performance run
            //
            for(int iter = 0; iter < number_hot_calls; ++iter)
            {
                CHECK_ROCSPARSE_ERROR(
                    rocsparse_spgemm(PARAMS(h_alpha_ptr, A, B, D, h_beta_ptr, C, dbuffer)));
            }

            gpu_solve_time_used = (get_time_us() - gpu_solve_time_used) / number_hot_calls;
            CHECK_HIP_ERROR(hipFree(dbuffer));
        }

        double gpu_gflops = csrgemm_gflop_count<T, I, J>(
                                M, h_alpha_ptr, hA.ptr, hA.ind, hB.ptr, h_beta_ptr, hD.ptr, hA.base)
                            / gpu_solve_time_used * 1e6;

        double gpu_gbyte = csrgemm_gbyte_count<T, I, J>(
                               M, N, K, dA.nnz, dB.nnz, C_nnz, dD.nnz, h_alpha_ptr, h_beta_ptr)
                           / gpu_solve_time_used * 1e6;

        std::cout.precision(2);
        std::cout.setf(std::ios::fixed);
        std::cout.setf(std::ios::left);

        std::cout << std::setw(12) << "opA" << std::setw(12) << "opB" << std::setw(12) << "M"
                  << std::setw(12) << "N" << std::setw(12) << "K" << std::setw(12) << "nnz_A"
                  << std::setw(12) << "nnz_B" << std::setw(12) << "nnz_C" << std::setw(12)
                  << "nnz_D" << std::setw(12) << "alpha" << std::setw(12) << "beta" << std::setw(12)
                  << "GFlop/s" << std::setw(12) << "GB/s" << std::setw(16) << "nnz msec"
                  << std::setw(16) << "gemm msec" << std::setw(12) << "iter" << std::setw(12)
                  << "verified" << std::endl;

        std::cout << std::setw(12) << rocsparse_operation2string(trans_A) << std::setw(12)
                  << rocsparse_operation2string(trans_B) << std::setw(12) << M << std::setw(12) << N
                  << std::setw(12) << K << std::setw(12) << dA.nnz << std::setw(12) << dB.nnz
                  << std::setw(12) << C_nnz << std::setw(12) << dD.nnz << std::setw(12) << h_alpha
                  << std::setw(12) << h_beta << std::setw(12) << gpu_gflops << std::setw(12)
                  << gpu_gbyte << std::setw(16) << gpu_analysis_time_used / 1e3 << std::setw(16)
                  << gpu_solve_time_used / 1e3 << std::setw(12) << number_hot_calls << std::setw(12)
                  << (arg.unit_check ? "yes" : "no") << std::endl;
    }
}

#define INSTANTIATE(ITYPE, JTYPE, TTYPE)                                                 \
    template void testing_spgemm_csr_bad_arg<ITYPE, JTYPE, TTYPE>(const Arguments& arg); \
    template void testing_spgemm_csr<ITYPE, JTYPE, TTYPE>(const Arguments& arg)

INSTANTIATE(int32_t, int32_t, float);
INSTANTIATE(int32_t, int32_t, double);
INSTANTIATE(int32_t, int32_t, rocsparse_float_complex);
INSTANTIATE(int32_t, int32_t, rocsparse_double_complex);
INSTANTIATE(int64_t, int32_t, float);
INSTANTIATE(int64_t, int32_t, double);
INSTANTIATE(int64_t, int32_t, rocsparse_float_complex);
INSTANTIATE(int64_t, int32_t, rocsparse_double_complex);
INSTANTIATE(int64_t, int64_t, float);
INSTANTIATE(int64_t, int64_t, double);
INSTANTIATE(int64_t, int64_t, rocsparse_float_complex);
INSTANTIATE(int64_t, int64_t, rocsparse_double_complex);
