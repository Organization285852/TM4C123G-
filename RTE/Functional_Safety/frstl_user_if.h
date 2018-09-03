/*! \cond
 * ==============================================================================
 *
 *                        
 *                          Copyright (C) 2013-2015 YOGITECH S.p.A. 
 *                          PROPRIETARY YOGITECH S.p.A., ALL RIGHTS RESERVED
 *
 *  This file is part of fRSTL_EVAL, an evaluation tool provided by YOGITECH to
 *  prototype the functional integration of YOGITECH’s fRSTL products. 
 *
 *  IMPORTANT SAFETY INFORMATION
 *  
 *  fRSTL_EVAL is an evaluation  version of the YOGITECH fRSTL product. It has
 *  no safety properties at all and it is distributed only to allow early integration
 *  and prototyping of safety applications integrating the fRSTL product. Only the
 *  functionality of the safety fRSTL product is reproduced, but the diagnostic
 *  capability is not in the fRSTL_EVAL code. In any case fRSTL_EVAL shall not be
 *  intended to be used for verification or signoff purpose in place of the fRSTL
 *  full-featured product.
 *  To get a license of fRSTL product and receive the product deliverables including
 *  source code please contact YOGITECH at frstl_arm@yogitech.com
 *  
 *  LEGAL NOTICE
 *  
 *  This code is proprietary information of YOGITECH, and is supplied subject to, and
 *  may be used, as general reference to YOGITECH fRSTL products.
 *  This code is provided "as is" and is subject to change without notice according to
 *  continuous product development and improvements. All product details and product
 *  usage described in this code are provided by YOGITECH S.p.A. in good faith.
 *  YOGITECH does not make, and expressly disclaims, any representations or warranties
 *  as to the completeness, accuracy, fitness for a particular purpose, or usefulness
 *  of the information contained in this code. 
 *  In no event will YOGITECH be liable for loss of profits, loss of revenues, loss of
 *  savings, loss of use, loss or corruption of data or any indirect, incidental, special,
 *  exemplary, punitive or consequential damages, whether under tort, contract or other
 *  theories of recovery in connection with the application of this code.
 *  YOGITECH does not warrant that use of this code will not infringe any third party
 *  rights.
 *   
 *  Do not reproduce in any form.
 *  This code is proprietary to YOGITECH SPA. Disclosure rights, copies, reproduction
 *  or distribution of this code are subject to specific agreement conditions
 *
 *
 *  Product release:             <_Product_Release_Label_>
 *
 *  Internal Revision Status:    <_Internal_Revision_Status_Label_>
 *
 *  File Revision:               $Revision: X.Y $
 *
 *------------------------------------------------------------------------------
 * \endcond
 */
/*!
 *
 * \file  frstl_user_if.h
 * \brief user application shall include this header
 *
 */

#ifndef FRSTLMx_USER_IF_H_
#define FRSTLMx_USER_IF_H_

/*----------------------------------------------------------------------------*/
/* DEFINES, MACROS AND STATIC VARIABLES                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------*/
/*---------------------  CM4  CPU --------------------------------*/
/*----------------------------------------------------------------*/
#ifdef FRSTL_CM4
#define FRSTLM4_TEST_SEGMENT_0_ID   ((unsigned char)0)
#define FRSTLM4_TEST_SEGMENT_1_ID   ((unsigned char)1)
#define FRSTLM4_TEST_SEGMENT_2_ID   ((unsigned char)2)
#define FRSTLM4_TEST_SEGMENT_3_ID   ((unsigned char)3)
#define FRSTLM4_TEST_SEGMENT_4_ID   ((unsigned char)4)
#define FRSTLM4_TEST_SEGMENT_5_ID   ((unsigned char)5)
#define FRSTLM4_TEST_SEGMENT_6_ID   ((unsigned char)6)
#define FRSTLM4_TEST_SEGMENT_7_ID   ((unsigned char)7)
#define FRSTLM4_TEST_SEGMENT_8_ID   ((unsigned char)8)
#define FRSTLM4_TEST_SEGMENT_9_ID   ((unsigned char)9)
#define FRSTLM4_TEST_SEGMENT_10_ID   ((unsigned char)10)
#define FRSTLM4_TEST_SEGMENT_11_ID   ((unsigned char)11)
#define FRSTLM4_TEST_SEGMENT_12_ID   ((unsigned char)12)
#define FRSTLM4_TEST_SEGMENT_13_ID   ((unsigned char)13)
#define FRSTLM4_TEST_SEGMENT_14_ID   ((unsigned char)14)
#define FRSTLM4_TEST_SEGMENT_15_ID   ((unsigned char)15)
#define FRSTLM4_TEST_SEGMENT_16_ID   ((unsigned char)16)
#define FRSTLM4_TEST_SEGMENT_17_ID   ((unsigned char)17)
#define FRSTLM4_TEST_SEGMENT_18_ID   ((unsigned char)18)
#define FRSTLM4_TEST_SEGMENT_19_ID   ((unsigned char)19)
#define FRSTLM4_TEST_SEGMENT_20_ID   ((unsigned char)20)
#define FRSTLM4_TEST_SEGMENT_21_ID   ((unsigned char)21)
#define FRSTLM4_TEST_SEGMENT_22_ID   ((unsigned char)22)
#define FRSTLM4_TEST_SEGMENT_23_ID   ((unsigned char)23)
#define FRSTLM4_TEST_SEGMENT_24_ID   ((unsigned char)24)
#define FRSTLM4_TEST_SEGMENT_25_ID   ((unsigned char)25)
#define FRSTLM4_TEST_SEGMENT_26_ID   ((unsigned char)26)
#define FRSTLM4_TEST_SEGMENT_27_ID   ((unsigned char)27)
#define FRSTLM4_TEST_SEGMENT_28_ID   ((unsigned char)28)
#define FRSTLM4_TEST_SEGMENT_29_ID   ((unsigned char)29)
#define FRSTLM4_TEST_SEGMENT_30_ID   ((unsigned char)30)

/*Test segments mnemonic definitions */

#define FRSTLM4_FIRST_TEST_ID       (FRSTLM4_TEST_SEGMENT_0_ID)
#define FRSTLM4_LAST_TEST_ID        (FRSTLM4_TEST_SEGMENT_30_ID)
#endif // FRSTL_CM4    CM4 CPU

/*Numbers for encoding test results */
#define FRSTLM4_PASSED_RESULT 0x78A5C3E1
#define FRSTLM4_FAILED_RESULT ~FRSTLM4_PASSED_RESULT

/*-------------------------------------------------------*/
/* -----  definition of online test module results       */
/*-------------------------------------------------------*/
typedef enum {
    e_OnlineTestResult_PASSED = FRSTLM4_PASSED_RESULT,
    e_OnlineTestResult_FAILED = FRSTLM4_FAILED_RESULT
} e_OnlineTestResult_t;

/*-------------------------------------------------------*/
/* -----  definition of stl bool type                    */
/*-------------------------------------------------------*/

/*! Boolean type definition */
typedef enum {
  STL_FALSE = (int)0xABCDABCD,
  STL_TRUE = (int)0x75601522
} stl_bool;

/*----------------------------------------------------------------------------*/
/* BODY                                                                       */
/*----------------------------------------------------------------------------*/

/*!
 * This is the entry point from the main application, and it is used to schedule
 * the execution of a test segment.
 *
 * \param TestSegmentID - The test identifier identifies the test to be executed
 * \return An error code: e_OnlineTestResult_FAILED in case of failure,
 * e_OnlineTestResult_PASSED otherwise
 */
e_OnlineTestResult_t frstl_run_test(const unsigned char u8_TestSegmentID);

/*!
 * Function to reset the status tracking of STL: to be called between
 *  the wtachdog reset and the next call to OnlineTestModule()
 */
void frstl_reset(void);

/*!
 * Registers the stl_run_ok status signal
 *
 * \param stl_run_ok - A double pointer that is used to return the address of
 * the run ok flag. This tells whether the STL has completed or not a run cycle
 * \return An error code: e_OnlineTestResult_FAILED in case of failure,
 * e_OnlineTestResult_PASSED otherwise
 */
e_OnlineTestResult_t frstl_register(const stl_bool** stl_run_ok);

/*!
 * This function schedules a test segment for execution, and checks the test 
 * segment to return the expected signature. It then sets the return value 
 * accordingly
 *
 * \param TestSegmentID - The test identifier
 * \return An error code: e_OnlineTestResult_FAILED in case of failure,
 * e_OnlineTestResult_PASSED otherwise
 */
e_OnlineTestResult_t frstl_run_and_update(const unsigned char u8_TestSegmentID);

#endif /*FRSTLMx_USER_IF_H_*/

