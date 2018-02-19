/*********************                                                        */
/*! \file RunReluplex.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz
 ** This file is part of the Reluplex project.
 ** Copyright (c) 2016-2017 by the authors listed in the file AUTHORS
 ** (in the top-level source directory) and their institutional affiliations.
 ** All rights reserved. See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **/

#ifndef __RunReluplex_h__
#define __RunReluplex_h__

#include "Reluplex.h"

class RunReluplex
{
public:
    double **currentAdversaryE ;

    int num_Node = 21;
    int num_AE = 0;
    int total = 9;

    RunReluplex() : _reluplex( NULL )
    {
        currentAdversaryE = new double*[total];
        for (int i = 0; i < total; i++) {
            currentAdversaryE[i] = new double[num_Node];
        }
    }

    ~RunReluplex()
    {
        if ( _reluplex )
            delete _reluplex;
    }

    void printCurrentAE(){
        printf("printCurrentAE in RunReluplex:\n");
        for (int j = 0; j < num_AE; ++j) {
            printf("This is a adversial example:\n");
            for (int k = 0; k < num_Node; ++k) {
                printf( "Variable %u : value = %.10lf \n", k , currentAdversaryE[j][k]);
            }
        }
    }

    // This is the simple example from the paper.
    void example1()
    {
        _reluplex = new Reluplex( 9 );

        _reluplex->setName( 0, "x1" );
        _reluplex->setName( 1, "x2b" );
        _reluplex->setName( 2, "x2f" );
        _reluplex->setName( 3, "x3b" );
        _reluplex->setName( 4, "x3f" );
        _reluplex->setName( 5, "x4" );
        _reluplex->setName( 6, "x5" );
        _reluplex->setName( 7, "x6" );
        _reluplex->setName( 8, "x7" );

        _reluplex->setLogging( true );

        _reluplex->initializeCell( 6, 2, 1.0 );
        _reluplex->initializeCell( 6, 4, 1.0 );
        _reluplex->initializeCell( 6, 5, -1.0 );
        _reluplex->initializeCell( 6, 6, -1.0 );

        _reluplex->initializeCell( 7, 0, 1.0 );
        _reluplex->initializeCell( 7, 1, -1.0 );
        _reluplex->initializeCell( 7, 7, -1.0 );

        _reluplex->initializeCell( 8, 0, 1.0 );
        _reluplex->initializeCell( 8, 3, 1.0 );
        _reluplex->initializeCell( 8, 8, -1.0 );

        _reluplex->markBasic( 6 );
        _reluplex->markBasic( 7 );
        _reluplex->markBasic( 8 );

        _reluplex->setLowerBound( 0, 0.0 );
        _reluplex->setUpperBound( 0, 1.0 );
        _reluplex->setLowerBound( 2, 0.0 );
        _reluplex->setLowerBound( 4, 0.0 );
        _reluplex->setLowerBound( 5, 0.5 );
        _reluplex->setUpperBound( 5, 1.0 );
        _reluplex->setLowerBound( 6, 0.0 );
        _reluplex->setUpperBound( 6, 0.0 );
        _reluplex->setLowerBound( 7, 0.0 );
        _reluplex->setUpperBound( 7, 0.0 );
        _reluplex->setLowerBound( 8, 0.0 );
        _reluplex->setUpperBound( 8, 0.0 );

        _reluplex->setLowerBound( 1, -9.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
        _reluplex->setUpperBound( 1, 9.0 );
        _reluplex->setUpperBound( 2, 9.0 );
        _reluplex->setLowerBound( 3, -9.0 );
        _reluplex->setUpperBound( 3, 9.0 );
        _reluplex->setUpperBound( 4, 9.0 );

        _reluplex->setReluPair( 1, 2 );
        _reluplex->setReluPair( 3, 4 );
    }

    // An unsolveable example
    void example2()
    {
        _reluplex = new Reluplex( 10 );

        _reluplex->setName( 0, "x1" );
        _reluplex->setName( 1, "x2b" );
        _reluplex->setName( 2, "x2f" );
        _reluplex->setName( 3, "x3b" );
        _reluplex->setName( 4, "x3f" );
        _reluplex->setName( 5, "x4" );
        _reluplex->setName( 6, "x5" );
        _reluplex->setName( 7, "x6" );
        _reluplex->setName( 8, "x7" );
        _reluplex->setName( 9, "x8" );

        _reluplex->setLogging( true );

        _reluplex->initializeCell( 6, 2, 1.0 );
        _reluplex->initializeCell( 6, 5, -1.0 );
        _reluplex->initializeCell( 6, 6, -1.0 );

        _reluplex->initializeCell( 7, 0, 1.0 );
        _reluplex->initializeCell( 7, 1, -1.0 );
        _reluplex->initializeCell( 7, 7, -1.0 );

        _reluplex->initializeCell( 8, 0, -1.0 );
        _reluplex->initializeCell( 8, 3, -1.0 );
        _reluplex->initializeCell( 8, 8, -1.0 );

        _reluplex->initializeCell( 9, 4, 1.0 );
        _reluplex->initializeCell( 9, 5, -1.0 );
        _reluplex->initializeCell( 9, 9, -1.0 );

        _reluplex->markBasic( 6 );
        _reluplex->markBasic( 7 );
        _reluplex->markBasic( 8 );
        _reluplex->markBasic( 9 );

        _reluplex->setLowerBound( 0, 0.0 );
        _reluplex->setUpperBound( 0, 1.0 );
        _reluplex->setLowerBound( 2, 0.0 );
        _reluplex->setLowerBound( 4, 0.0 );
        _reluplex->setLowerBound( 5, 0.5 );
        _reluplex->setUpperBound( 5, 1.0 );
        _reluplex->setLowerBound( 6, 0.0 );
        _reluplex->setUpperBound( 6, 0.0 );
        _reluplex->setLowerBound( 7, 0.0 );
        _reluplex->setUpperBound( 7, 0.0 );
        _reluplex->setLowerBound( 8, 0.0 );
        _reluplex->setUpperBound( 8, 0.0 );
        _reluplex->setLowerBound( 9, 0.0 );
        _reluplex->setUpperBound( 9, 0.0 );

        _reluplex->setLowerBound( 1, -9.0 );
        _reluplex->setUpperBound( 1, 9.0 );
        _reluplex->setUpperBound( 2, 9.0 );
        _reluplex->setLowerBound( 3, -9.0 );
        _reluplex->setUpperBound( 3, 9.0 );
        _reluplex->setUpperBound( 4, 9.0 );

        _reluplex->setReluPair( 1, 2 );
        _reluplex->setReluPair( 3, 4 );
    }

//    void example3()
//    {
//        _reluplex = new Reluplex( 21 );
//
//        _reluplex->setName( 0, "x1" );
//        _reluplex->setName( 1, "x2b" );
//        _reluplex->setName( 2, "x2f" );
//        _reluplex->setName( 3, "x3b" );
//        _reluplex->setName( 4, "x3f" );
//        _reluplex->setName( 5, "x4b" );
//        _reluplex->setName( 6, "x4f" );
//        _reluplex->setName( 7, "x5b" );
//        _reluplex->setName( 8, "x5f" );
//        _reluplex->setName( 9, "x6b" );
//        _reluplex->setName( 10, "x6f" );
//        _reluplex->setName( 11, "x7b" );
//        _reluplex->setName( 12, "x7f" );
//        _reluplex->setName( 13, "x8" );
//        _reluplex->setName( 14, "a1" );
//        _reluplex->setName( 15, "a2" );
//        _reluplex->setName( 16, "a3" );
//        _reluplex->setName( 17, "a4" );
//        _reluplex->setName( 18, "a5" );
//        _reluplex->setName( 19, "a6" );
//        _reluplex->setName( 20, "a7" );
//
//        _reluplex->setLogging( true );
//
//        _reluplex->initializeCell( 14, 0, -1.0 );
//        _reluplex->initializeCell( 14, 1, 1.0 );
//        _reluplex->initializeCell( 14, 14, -1.0 );
//
//        _reluplex->initializeCell( 15, 0, 1.0 );
//        _reluplex->initializeCell( 15, 3, 1.0 );
//        _reluplex->initializeCell( 15, 15, -1.0 );
//
//        _reluplex->initializeCell( 16, 0, -1.0 );
//        _reluplex->initializeCell( 16, 5, 1.0 );
//        _reluplex->initializeCell( 16, 16, -1.0 );
//
//        _reluplex->initializeCell( 17, 2, -1.0 );
//        _reluplex->initializeCell( 17, 4, -2.0 );
//        _reluplex->initializeCell( 17, 6, -3.0 );
//        _reluplex->initializeCell( 17, 7, 1.0 );
//        _reluplex->initializeCell( 17, 17, -1.0 );
//
//        _reluplex->initializeCell( 18, 2, -2.0 );
//        _reluplex->initializeCell( 18, 4, -1.0 );
//        _reluplex->initializeCell( 18, 6, -1.0 );
//        _reluplex->initializeCell( 18, 9, 1.0 );
//        _reluplex->initializeCell( 18, 18, -1.0 );
//
//        _reluplex->initializeCell( 19, 2, -1.0 );
//        _reluplex->initializeCell( 19, 4, -2.0 );
//        _reluplex->initializeCell( 19, 6, -2.0 );
//        _reluplex->initializeCell( 19, 11, 1.0 );
//        _reluplex->initializeCell( 19, 19, -1.0 );
//
//        _reluplex->initializeCell( 20, 8, -3.0 );
//        _reluplex->initializeCell( 20, 10, -2.0 );
//        _reluplex->initializeCell( 20, 12, -1.0 );
//        _reluplex->initializeCell( 20, 13, 1.0 );
//        _reluplex->initializeCell( 20, 20, -1.0 );
//
//
//        _reluplex->markBasic( 14 );
//        _reluplex->markBasic( 15 );
//        _reluplex->markBasic( 16 );
//        _reluplex->markBasic( 17 );
//        _reluplex->markBasic( 18 );
//        _reluplex->markBasic( 19 );
//        _reluplex->markBasic( 20 );
//
//        _reluplex->setReluPair( 1, 2 );
//        _reluplex->setReluPair( 3, 4 );
//        _reluplex->setReluPair( 5, 6 );
//        _reluplex->setReluPair( 7, 8 );
//        _reluplex->setReluPair( 9, 10 );
//        _reluplex->setReluPair( 11, 12 );
//
//        // set bound of basic
//        _reluplex->setLowerBound( 14, 0.0 );
//        _reluplex->setUpperBound( 14, 0.0 );
//        _reluplex->setLowerBound( 15, 0.0 );
//        _reluplex->setUpperBound( 15, 0.0 );
//        _reluplex->setLowerBound( 16, 0.0 );
//        _reluplex->setUpperBound( 16, 0.0 );
//        _reluplex->setLowerBound( 17, 0.0 );
//        _reluplex->setUpperBound( 17, 0.0 );
//        _reluplex->setLowerBound( 18, 0.0 );
//        _reluplex->setUpperBound( 18, 0.0 );
//        _reluplex->setLowerBound( 19, 0.0 );
//        _reluplex->setUpperBound( 19, 0.0 );
//        _reluplex->setLowerBound( 20, 0.0 );
//        _reluplex->setUpperBound( 20, 0.0 );
//
//        // set b bound
//        _reluplex->setLowerBound( 1, -9999.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
//        _reluplex->setUpperBound( 1, 9999.0 );
//        _reluplex->setLowerBound( 3, -9999.0 );
//        _reluplex->setUpperBound( 3, 9999.0 );
//        _reluplex->setLowerBound( 5, -9999.0 );
//        _reluplex->setUpperBound( 5, 9999.0 );
//        _reluplex->setLowerBound( 7, -9999.0 );
//        _reluplex->setUpperBound( 7, 9999.0 );
//        _reluplex->setLowerBound( 9, -9999.0 );
//        _reluplex->setUpperBound( 9, 9999.0 );
//        _reluplex->setLowerBound( 11, -9999.0 );
//        _reluplex->setUpperBound( 11, 9999.0 );
//
//        // set f bound
//        _reluplex->setLowerBound( 2, 0.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
//        _reluplex->setUpperBound( 2, 9999.0 );
//        _reluplex->setLowerBound( 4, 0.0 );
//        _reluplex->setUpperBound( 4, 9999.0 );
//        _reluplex->setLowerBound( 6, 0.0 );
//        _reluplex->setUpperBound( 6, 9999.0 );
//        _reluplex->setLowerBound( 8, 0.0 );
//        _reluplex->setUpperBound( 8, 9999.0 );
//        _reluplex->setLowerBound( 10, 0.0 );
//        _reluplex->setUpperBound( 10, 9999.0 );
//        _reluplex->setLowerBound( 12, 0.0 );
//        _reluplex->setUpperBound( 12, 9999.0 );
//
//
//        // set input & output
//
//        _reluplex->setLowerBound( 0, 0.5 );
//        _reluplex->setUpperBound( 0, 1.0 );
//        _reluplex->setLowerBound( 13, 0.0 );
//        _reluplex->setUpperBound( 13, 100.0 );
//
//    }

    void example3(double assign = 0.5, double delta = 0.1 )
    {
        _reluplex = new Reluplex( 21 );

        _reluplex->setName( 0, "x1" );
        _reluplex->setName( 1, "x2b" );
        _reluplex->setName( 2, "x2f" );
        _reluplex->setName( 3, "x3b" );
        _reluplex->setName( 4, "x3f" );
        _reluplex->setName( 5, "x4b" );
        _reluplex->setName( 6, "x4f" );
        _reluplex->setName( 7, "x5b" );
        _reluplex->setName( 8, "x5f" );
        _reluplex->setName( 9, "x6b" );
        _reluplex->setName( 10, "x6f" );
        _reluplex->setName( 11, "x7b" );
        _reluplex->setName( 12, "x7f" );
        _reluplex->setName( 13, "x8" );
        _reluplex->setName( 14, "a1" );
        _reluplex->setName( 15, "a2" );
        _reluplex->setName( 16, "a3" );
        _reluplex->setName( 17, "a4" );
        _reluplex->setName( 18, "a5" );
        _reluplex->setName( 19, "a6" );
        _reluplex->setName( 20, "a7" );

        _reluplex->setLogging( true );

        _reluplex->initializeCell( 14, 0, -1.0 );
        _reluplex->initializeCell( 14, 1, 1.0 );
        _reluplex->initializeCell( 14, 14, -1.0 );

        _reluplex->initializeCell( 15, 0, 1.0 );
        _reluplex->initializeCell( 15, 3, 1.0 );
        _reluplex->initializeCell( 15, 15, -1.0 );

        _reluplex->initializeCell( 16, 0, -1.0 );
        _reluplex->initializeCell( 16, 5, 1.0 );
        _reluplex->initializeCell( 16, 16, -1.0 );

        _reluplex->initializeCell( 17, 2, -1.0 );
        _reluplex->initializeCell( 17, 4, -2.0 );
        _reluplex->initializeCell( 17, 6, -3.0 );
        _reluplex->initializeCell( 17, 7, 1.0 );
        _reluplex->initializeCell( 17, 17, -1.0 );

        _reluplex->initializeCell( 18, 2, -2.0 );
        _reluplex->initializeCell( 18, 4, -1.0 );
        _reluplex->initializeCell( 18, 6, -1.0 );
        _reluplex->initializeCell( 18, 9, 1.0 );
        _reluplex->initializeCell( 18, 18, -1.0 );

        _reluplex->initializeCell( 19, 2, -1.0 );
        _reluplex->initializeCell( 19, 4, -2.0 );
        _reluplex->initializeCell( 19, 6, -2.0 );
        _reluplex->initializeCell( 19, 11, 1.0 );
        _reluplex->initializeCell( 19, 19, -1.0 );

        _reluplex->initializeCell( 20, 8, -3.0 );
        _reluplex->initializeCell( 20, 10, -2.0 );
        _reluplex->initializeCell( 20, 12, -1.0 );
        _reluplex->initializeCell( 20, 13, 1.0 );
        _reluplex->initializeCell( 20, 20, -1.0 );


        _reluplex->markBasic( 14 );
        _reluplex->markBasic( 15 );
        _reluplex->markBasic( 16 );
        _reluplex->markBasic( 17 );
        _reluplex->markBasic( 18 );
        _reluplex->markBasic( 19 );
        _reluplex->markBasic( 20 );

        _reluplex->setReluPair( 1, 2 );
        _reluplex->setReluPair( 3, 4 );
        _reluplex->setReluPair( 5, 6 );
        _reluplex->setReluPair( 7, 8 );
        _reluplex->setReluPair( 9, 10 );
        _reluplex->setReluPair( 11, 12 );

        // set bound of basic
        _reluplex->setLowerBound( 14, 0.0 );
        _reluplex->setUpperBound( 14, 0.0 );
        _reluplex->setLowerBound( 15, 0.0 );
        _reluplex->setUpperBound( 15, 0.0 );
        _reluplex->setLowerBound( 16, 0.0 );
        _reluplex->setUpperBound( 16, 0.0 );
        _reluplex->setLowerBound( 17, 0.0 );
        _reluplex->setUpperBound( 17, 0.0 );
        _reluplex->setLowerBound( 18, 0.0 );
        _reluplex->setUpperBound( 18, 0.0 );
        _reluplex->setLowerBound( 19, 0.0 );
        _reluplex->setUpperBound( 19, 0.0 );
        _reluplex->setLowerBound( 20, 0.0 );
        _reluplex->setUpperBound( 20, 0.0 );

        // set b bound
        _reluplex->setLowerBound( 1, -9999.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
        _reluplex->setUpperBound( 1, 9999.0 );
        _reluplex->setLowerBound( 3, -9999.0 );
        _reluplex->setUpperBound( 3, 9999.0 );
        _reluplex->setLowerBound( 5, -9999.0 );
        _reluplex->setUpperBound( 5, 9999.0 );
        _reluplex->setLowerBound( 7, -9999.0 );
        _reluplex->setUpperBound( 7, 9999.0 );
        _reluplex->setLowerBound( 9, -9999.0 );
        _reluplex->setUpperBound( 9, 9999.0 );
        _reluplex->setLowerBound( 11, -9999.0 );
        _reluplex->setUpperBound( 11, 9999.0 );

        // set f bound
        _reluplex->setLowerBound( 2, 0.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
        _reluplex->setUpperBound( 2, 9999.0 );
        _reluplex->setLowerBound( 4, 0.0 );
        _reluplex->setUpperBound( 4, 9999.0 );
        _reluplex->setLowerBound( 6, 0.0 );
        _reluplex->setUpperBound( 6, 9999.0 );
        _reluplex->setLowerBound( 8, 0.0 );
        _reluplex->setUpperBound( 8, 9999.0 );
        _reluplex->setLowerBound( 10, 0.0 );
        _reluplex->setUpperBound( 10, 9999.0 );
        _reluplex->setLowerBound( 12, 0.0 );
        _reluplex->setUpperBound( 12, 9999.0 );


        // set input & output

        _reluplex->setLowerBound( 0, assign - delta );
        _reluplex->setUpperBound( 0, 1.0 );
        _reluplex->setLowerBound( 13, 0.0 );
        _reluplex->setUpperBound( 13, 100.0 );

    }

    void go()
    {
        // Choose between the 2 available examples
//        example1();
//         example2();
         example3(0.6);

        _reluplex->setLogging( true );
        _reluplex->setDumpStates( true );

        try
        {
            printf("num_AE: %d \n", num_AE);
            Reluplex::FinalStatus result = _reluplex->solve();
            if ( result == Reluplex::SAT ){
                printf( "\n*** Solved! ***\n" );

                /*** add by lzs **/

                for (int i = 0; i < num_Node; i++) {
                    currentAdversaryE[num_AE][i] = _reluplex->_assignment[i];
                }
                num_AE ++;

                //// -----------------

                example3(_reluplex->_assignment[0]);
                _reluplex->setLogging( true );
                _reluplex->setDumpStates( true );
                try {

                    printf("num_AE: %d \n", num_AE);
                    Reluplex::FinalStatus result = _reluplex->solve();
                    if (result == Reluplex::SAT) {
                        printf("\n*** Solved! ***\n");

                        for (int i = 0; i < num_Node; i++) {
                            currentAdversaryE[num_AE][i] = _reluplex->_assignment[i];
                        }
                        num_AE ++;

                    }
                    else if ( result == Reluplex::UNSAT )
                        printf( "\n*** Can't Solve ***\n" );
                    else if ( result == Reluplex::ERROR )
                        printf( "Reluplex error!\n" );
                    else
                        printf( "Reluplex not done (quit called?)\n" );
                }
                catch ( const Error &e )
                {
                    if ( e.code() == Error::STACK_IS_EMPTY )
                        printf( "\n*** Can't Solve (STACK EMPTY) ***\n" );
                    else if ( e.code() == Error::UPPER_LOWER_INVARIANT_VIOLATED )
                        printf( "\n*** Can't Solve (UPPER_LOWER_INVARIANT_VIOLATED) ***\n" );
                    else
                    {
                        throw e;
                    }
                }
                // 每次调用progress前都要存储此时_reluplex对象的状态，如果findPivotCandidate中有多个候选者，则将进入这段代码之前
                // 存储的_reluplex对象的状态正式投入使用，在运行完一次得到解答之后，回到算法中当时运行progress时的状态，
                // 继续尝试运行，并在findPivotCandidate之时排除上一次所选的候选者

                /*** add end ***/
            }
            else if ( result == Reluplex::UNSAT )
                printf( "\n*** Can't Solve ***\n" );
            else if ( result == Reluplex::ERROR )
                printf( "Reluplex error!\n" );
            else
                printf( "Reluplex not done (quit called?)\n" );

                printCurrentAE();

        }
        catch ( const Error &e )
        {
            if ( e.code() == Error::STACK_IS_EMPTY )
                printf( "\n*** Can't Solve (STACK EMPTY) ***\n" );
            else if ( e.code() == Error::UPPER_LOWER_INVARIANT_VIOLATED )
                printf( "\n*** Can't Solve (UPPER_LOWER_INVARIANT_VIOLATED) ***\n" );
            else
            {
                throw e;
            }
        }
    }

private:
    Reluplex *_reluplex;
};

#endif // __RunReluplex_h__

//
// Local Variables:
// compile-command: "make -C . "
// tags-file-name: "./TAGS"
// c-basic-offset: 4
// End:
//
