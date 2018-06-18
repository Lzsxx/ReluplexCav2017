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

    unsigned num_Node = 21;
    unsigned num_AE = 0;
    unsigned total = 9;

    RunReluplex() : _reluplex( NULL )
    {
        currentAdversaryE = new double*[total];
        for (unsigned i = 0; i < total; i++) {
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
        for (unsigned j = 0; j < num_AE; ++j) {
            printf("\nThis is a adversial example:\n");
            for (unsigned k = 0; k < num_Node; ++k) {
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

    void example1_leaky()
    {
        _reluplex = new Reluplex( 9 );
        _reluplex->setLeakyValue(0.5);

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

        _reluplex->setLowerBound( 0, 0.6 );
        _reluplex->setUpperBound( 0, 1.0 );
        _reluplex->setLowerBound( 5, 0.2 );
        _reluplex->setUpperBound( 5, 0.3 );
        _reluplex->setLowerBound( 6, 0.0 );
        _reluplex->setUpperBound( 6, 0.0 );
        _reluplex->setLowerBound( 7, 0.0 );
        _reluplex->setUpperBound( 7, 0.0 );
        _reluplex->setLowerBound( 8, 0.0 );
        _reluplex->setUpperBound( 8, 0.0 );

        _reluplex->setLowerBound( 1, -9999.0 );    // 为了例子简便，用-9表示负无穷，+9表示正无穷
        _reluplex->setUpperBound( 1, 9999.0 );
        _reluplex->setLowerBound( 2, -9999.0 );
        _reluplex->setUpperBound( 2, 9999.0 );
        _reluplex->setLowerBound( 3, -9999.0 );
        _reluplex->setUpperBound( 3, 9999.0 );
        _reluplex->setLowerBound( 4, -9999.0 );
        _reluplex->setUpperBound( 4, 9999.0 );

        _reluplex->setReluPair( 1, 2 );
        _reluplex->setReluPair( 3, 4 );
    }

    void go()
    {
        // Choose between the 2 available examples
//        example1();
        example1_leaky();

        _reluplex->setLogging( true );
        _reluplex->setDumpStates( true );

        //// add by lzs
        /******** change! **********/
        double **currentAdversaryE ;
        unsigned num_Node = 1 + 1; //【不可修改，但要修改Reluplex里的solve的indexToVar部分】  输入层元素个数+输出层元素个数
        unsigned num_AE = 0;    // 当前已取得的对抗样本数
        unsigned total = 5; //【可修改】申请出来存放对抗样本的空间，想要多少个就为多少，不得小于num_Expected_AE
        unsigned num_Expected_AE = 5;   //【可修改】单轮runner期望找到的对抗样本数

        currentAdversaryE = new double*[total];
        for (unsigned i = 0; i < total; i++) {
            currentAdversaryE[i] = new double[num_Node];
        }
        //// add end

        try
        {
            Reluplex::FinalStatus result = _reluplex->solve(currentAdversaryE, num_AE, num_Node, num_Expected_AE);
            if ( result == Reluplex::SAT ){
                printf( "\n*** Solved! ***\n" );

                //// add by lzs
                printf("\n\n===========================\n");
                printf("Summary!!! printCurrentAE in main:SAT \n");
                for (unsigned j = num_AE; j > 0; --j) {  //倒序输出，为了方便与上面比较，同时unsigned >=0恒为true，所以这里只>0,后面每次使用j时-1即可
                    printf("\nThis is a adversial example: %u \n", num_AE - j + 1);
                    for (unsigned k = 0; k < num_Node; ++k) {
                        double assignment = currentAdversaryE[j-1][k];
                        /**** change! 1 means inputLayerSize****/
                        if( k < 1){ // 2
                            printf( "input[%u] = %lf.\n", k, assignment );
                        } else{
                            printf( "output[%u] = %.10lf.\n", k, assignment );
                        }
                    }
                    printf("\n-------------------------\n");
                }
                //// add end
            } else if ( result == Reluplex::UNSAT ){
                printf( "\n*** Can't Solve ***\n" );
            } else if ( result == Reluplex::ERROR ){
                printf( "Reluplex error!\n" );
            } else if ( result == Reluplex::NOT_DONE){
                printf( "Reluplex not done (quit called?)\n" );
            }
                /*** add by lzs ***/
            else{
                printf("\n----- printStatistics():Reluplex SAT But Then ... ----\n");
                _reluplex->printStatistics();
                printf("\n==========================\n");
                printf("Summary!!! printCurrentAE in main: other result \n");
                for (unsigned j = num_AE; j > 0; --j) {  //倒序输出，为了方便与上面比较，同时unsigned >=0恒为true，所以这里只>0,后面每次使用j时-1即可
                    printf("\nThis is a adversial example: %u \n", num_AE - j + 1);
                    for (unsigned k = 0; k < num_Node; ++k) {
                        double assignment = currentAdversaryE[j-1][k];
                        /**** change! 1 means inputLayerSize****/
                        if( k < 1){ // 2
                            printf( "input[%u] = %lf.\n", k, assignment );
                        } else{
                            printf( "output[%u] = %.10lf.\n", k, assignment );
                        }
                    }
                    printf("\n-------------------------\n");
                }
            }
            /*** add end ***/
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
    void go_temp()
    {
        // Choose between the 2 available examples
//        example1();
        example1_leaky();

        _reluplex->setLogging( true );
        _reluplex->setDumpStates( true );

        try
        {
            Reluplex::FinalStatus result = _reluplex->solve();
            if ( result == Reluplex::SAT )
                printf( "\n*** Solved! ***\n" );
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
    }
    void print_result(Reluplex::FinalStatus result){
        switch ( result )
        {
            case Reluplex::SAT:
                printf("Reluplex::SAT \n");
                break;
            case Reluplex::UNSAT:
                printf("Reluplex::UNSAT \n");
                break;
            case Reluplex::ERROR:
                printf("Reluplex::ERROR \n");
                break;
            case Reluplex::NOT_DONE:
                printf("Reluplex::NOT_DONE \n");
                break;
            case Reluplex::SAT_BUT_THEN_UNSAT:
                printf("Reluplex::SAT_BUT_THEN_UNSAT \n");
                break;
            case Reluplex::SAT_BUT_THEN_ERROR:
                printf("Reluplex::SAT_BUT_THEN_ERROR \n");
                break;
            case Reluplex::SAT_BUT_THEN_NOT_DONE:
                printf("Reluplex::SAT_BUT_THEN_NOT_DONE \n");
                break;
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
