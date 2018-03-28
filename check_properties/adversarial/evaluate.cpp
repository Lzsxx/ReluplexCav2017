/*********************                                                        */
/*! \file main.cpp
** \verbatim
** Top contributors (to current version):
**   Guy Katz
** This file is part of the Reluplex project.
** Copyright (c) 2016-2017 by the authors listed in the file AUTHORS
** (in the top-level source directory) and their institutional affiliations.
** All rights reserved. See the file COPYING in the top-level source
** directory for licensing information.\endverbatim
**/

#include <cstdio>
#include <signal.h>

#include "AcasNeuralNetwork.h"
#include "File.h"
#include "Reluplex.h"
#include "String.h"

const char *FULL_NET_PATH = "./nnet/ACASXU_run2a_1_1_batch_2000.nnet";

struct Index
{
    Index( unsigned newRow, unsigned newCol, unsigned newF )
        : row( newRow ), col( newCol ), f( newF )
    {
    }

    unsigned row;
    unsigned col;
    bool f;

    bool operator<( const Index &other ) const
    {
        if ( row != other.row )
            return row < other.row;
        if ( col != other.col )
            return col < other.col;

        if ( !f && other.f )
            return true;
        if ( f && !other.f )
            return false;

        return false;
    }
};

double normalizeInput( unsigned inputIndex, double value, AcasNeuralNetwork &neuralNetwork )
{
    double min = neuralNetwork._network->mins[inputIndex];
    double max = neuralNetwork._network->maxes[inputIndex];
    double mean = neuralNetwork._network->means[inputIndex];
    double range = neuralNetwork._network->ranges[inputIndex];

    if ( value < min )
        value = min;
    else if ( value > max )
        value = max;

    return ( value - mean ) / range;
}

double unnormalizeInput( unsigned inputIndex, double value, AcasNeuralNetwork &neuralNetwork )
{
    double mean = neuralNetwork._network->means[inputIndex];
    double range = neuralNetwork._network->ranges[inputIndex];

    return ( value * range ) + mean;
}

double unnormalizeOutput( double output, AcasNeuralNetwork &neuralNetwork )
{
    int inputSize = neuralNetwork._network->inputSize;
    double mean = neuralNetwork._network->means[inputSize];
    double range = neuralNetwork._network->ranges[inputSize];

    return ( output - mean ) / range;
}

double normalizeOutput( double output, AcasNeuralNetwork &neuralNetwork )
{
    int inputSize = neuralNetwork._network->inputSize;
    double mean = neuralNetwork._network->means[inputSize];
    double range = neuralNetwork._network->ranges[inputSize];

    return ( output * range ) + mean;
}

void getFixedInputs( Vector<double> &fixedInputs, unsigned pointToUse )
{
    switch ( pointToUse )
    {
    case 0:
        fixedInputs = { 0, 0, 0, 0, 0 };
        break;

    case 1:
        fixedInputs = { 0.2, -0.1, 0, -0.3, 0.4 };
        break;

    case 2:
        fixedInputs = { 0.45, -0.23, -0.4, 0.12, 0.33 };
        break;

    case 3:
        fixedInputs = { -0.2, -0.25, -0.5, -0.3, -0.44 };
        break;

    case 4:
        fixedInputs = { 0.61, 0.36, 0.0, 0.0, -0.24 };
        break;
    }
}

void getFixedOutputs( const Vector<double> &fixedInputs,
                      Vector<double> &fixedOutputs,
                      unsigned outputLayerSize,
                      AcasNeuralNetwork &network )
{
    network.evaluate( fixedInputs, fixedOutputs, outputLayerSize );
}

void findMinimal( Vector<double> &fixedOutputs, unsigned &minimal )
{
    minimal = 0;

    for ( unsigned i = 1; i < fixedOutputs.size(); ++i )
        if ( fixedOutputs[i] < fixedOutputs[minimal] )
            minimal = i;
}

Reluplex *lastReluplex = NULL;

void got_signal( int )
{
    printf( "Got signal\n" );

    if ( lastReluplex )
    {
        lastReluplex->quit();
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

bool advMain( int argc, char **argv, unsigned inputPoint, double inputDelta, unsigned runnerUp )
{
    String networkPath = FULL_NET_PATH;
    char *finalOutputFile;

    if ( argc < 2 )
        finalOutputFile = NULL;
    else
        finalOutputFile = argv[1];

//    printf("\nfinalOutputFile:%s\n",finalOutputFile);  // 指向summary.txt文件，只有这一个参数，argv[2]指向null, argv[0]指向./check_properties/bin/adversarial.elf

    //读取网络
    AcasNeuralNetwork neuralNetwork( networkPath.ascii() );

    // 层数、输出层的节点数
    unsigned numLayersInUse = neuralNetwork.getNumLayers() + 1;
    unsigned outputLayerSize = neuralNetwork.getLayerSize( numLayersInUse - 1 );

    printf( "Num layers in use: %u\n", numLayersInUse );
    printf( "Output layer size: %u\n", outputLayerSize );

    //输入层的节点数
    unsigned inputLayerSize = neuralNetwork.getLayerSize( 0 );

    // 中间节点ReLU的个数
    unsigned numReluNodes = 0;
    for ( unsigned i = 1; i < numLayersInUse - 1; ++i )
        numReluNodes += neuralNetwork.getLayerSize( i );

    // 3种节点数相加就是节点总数
    printf( "Input nodes = %u, relu nodes = %u, output nodes = %u\n", inputLayerSize, numReluNodes, outputLayerSize );

    Vector<double> fixedInputs;
    Vector<double> fixedOutputs;

    // 输入层 节点的取值
    getFixedInputs( fixedInputs, inputPoint );

    // 输出层计算后 节点的取值
    getFixedOutputs( fixedInputs, fixedOutputs, outputLayerSize, neuralNetwork );

    unsigned minimal;
    findMinimal( fixedOutputs, minimal );   //找到output中最小值的下标

    if ( runnerUp >= minimal )  // runnerUp取值为0~3，如果小于minimal, 则保持不变，目的是minimal和其他index的值做对比
        ++runnerUp;             // 当runnerUp == minimal时，要跳过，所以+1，
                                // 由于runnerUp每次进来都会被+1.而一旦在 == 时被+1，后面进来的runnerUp所代表的值都已经被测试过
                                // 就应该继续往下移动一位，所以当 >= 时，一律需要将runnerUp ++ , 以获得正确的下标

    printf( "Inputs are: \n" );
    for ( unsigned i = 0; i < fixedInputs.size(); ++i )
    {
        printf( "\tinput[%u] = %lf\n", i, fixedInputs[i] );
    }

    printf( "Outputs are: \n" );
    for ( unsigned i = 0; i < fixedOutputs.size(); ++i )
    {
        printf( "\toutput[%u] = %lf\n", i, fixedOutputs[i] );
    }

    printf( "minimal: %u. runner up: %u\n", minimal, runnerUp );

    // Total size of the tableau:
    //   1. Input vars appear once
    //   2. Each internal var has a B instance, an F instance, and an auxiliary var for the B equation
    //   3. Each output var has an instance and an auxiliary var for its equation
    //   4. A single variable for the output constraints
    //   5. A single variable for the constants

    // Reluplex( unsigned numVariables, char *finalOutputFile = NULL, String reluplexName = "" )

    Reluplex reluplex( inputLayerSize + ( 3 * numReluNodes ) + ( 2 * outputLayerSize ) + 1 + 1,
                       finalOutputFile,
                       Stringf( "Point_%u_Delta_%.5lf_runnerUp_%u", inputPoint, inputDelta, runnerUp ) );

    lastReluplex = &reluplex;

    Map<Index, unsigned> nodeToVars;
    Map<Index, unsigned> nodeToAux;

    // We want to group variable IDs by layers.
    // The order is: f's from layer i, b's from layer i+1, aux variable for i+1, and repeat

    for ( unsigned i = 1; i < numLayersInUse; ++i )
    {
        unsigned currentLayerSize;
        if ( i + 1 == numLayersInUse )
            currentLayerSize = outputLayerSize;
        else
            currentLayerSize = neuralNetwork.getLayerSize( i );

        unsigned previousLayerSize = neuralNetwork.getLayerSize( i - 1 );

        // First add the f's from layer i-1
        for ( unsigned j = 0; j < previousLayerSize; ++j )
        {
            unsigned newIndex;

            newIndex = nodeToVars.size() + nodeToAux.size();
            nodeToVars[Index(i - 1, j, true)] = newIndex;
        }

        // Now add the b's from layer i
        for ( unsigned j = 0; j < currentLayerSize; ++j )
        {
            unsigned newIndex;

            newIndex = nodeToVars.size() + nodeToAux.size();
            nodeToVars[Index(i, j, false)] = newIndex;
        }

        // And now the aux variables from layer i
        for ( unsigned j = 0; j < currentLayerSize; ++j )
        {
            unsigned newIndex;

            newIndex = nodeToVars.size() + nodeToAux.size();
            nodeToAux[Index(i, j, false)] = newIndex;
        }
    }

    unsigned constantVar = nodeToVars.size() + nodeToAux.size();    // 这个变量的作用未知

    unsigned outputSlackVar = constantVar + 1;

    // Set bounds for constant var
    reluplex.setLowerBound( constantVar, 1.0 );
    reluplex.setUpperBound( constantVar, 1.0 );

    // Set bounds for inputs
    for ( unsigned i = 0; i < inputLayerSize ; ++i )
    {
        double realMax =
            ( neuralNetwork._network->maxes[i] - neuralNetwork._network->means[i] )
            / ( neuralNetwork._network->ranges[i] );
        double realMin =
            ( neuralNetwork._network->mins[i] - neuralNetwork._network->means[i] )
            / ( neuralNetwork._network->ranges[i] );

        double min = fixedInputs[i] - inputDelta;
        if ( min < realMin )
            min = realMin;

        double max = fixedInputs[i] + inputDelta;
        if ( max > realMax )
            max = realMax;

        printf( "Bounds for input %u: [ %.10lf, %.10lf ]\n", i, min, max );

        reluplex.setLowerBound( nodeToVars[Index(0, i, true)], min );
        reluplex.setUpperBound( nodeToVars[Index(0, i, true)], max );
    }

    // Set bounds for the output slack var. It's minimal - runnerUp,
    // so we want it to be positive - i.e., runner up scored lower.
    reluplex.setLowerBound( outputSlackVar, 0.0 );
    reluplex.markBasic( outputSlackVar );

    // Declare relu pairs and set bounds
    for ( unsigned i = 1; i < numLayersInUse - 1; ++i )
    {
        for ( unsigned j = 0; j < neuralNetwork.getLayerSize( i ); ++j )
        {
            unsigned b = nodeToVars[Index(i, j, false)];
            unsigned f = nodeToVars[Index(i, j, true)];

            reluplex.setReluPair( b, f );
            reluplex.setLowerBound( f, 0.0 );
        }
    }

    printf( "Number of auxiliary variables: %u\n", nodeToAux.size() );

    // Mark all aux variables as basic and set their bounds to zero
    for ( const auto &it : nodeToAux )
    {
        reluplex.markBasic( it.second );
        reluplex.setLowerBound( it.second, 0.0 );
        reluplex.setUpperBound( it.second, 0.0 );
    }

    // Populate the table
    for ( unsigned layer = 0; layer < numLayersInUse - 1; ++layer )
    {
        unsigned targetLayerSize;
        if ( layer + 2 == numLayersInUse )
            targetLayerSize = outputLayerSize;
        else
            targetLayerSize = neuralNetwork.getLayerSize( layer + 1 );

        for ( unsigned target = 0; target < targetLayerSize; ++target )
        {
            // This aux var will bind the F's from the previous layer to the B of this node.
            unsigned auxVar = nodeToAux[Index(layer + 1, target, false)];
            reluplex.initializeCell( auxVar, auxVar, -1 );

            unsigned bVar = nodeToVars[Index(layer + 1, target, false)];
            reluplex.initializeCell( auxVar, bVar, -1 );

            for ( unsigned source = 0; source < neuralNetwork.getLayerSize( layer ); ++source )
            {
                unsigned fVar = nodeToVars[Index(layer, source, true)];
                reluplex.initializeCell
                    ( auxVar,
                      fVar,
                      neuralNetwork.getWeight( layer, source, target ) );
            }

            // Add the bias via the constant var
            reluplex.initializeCell( auxVar,
                                     constantVar,
                                     neuralNetwork.getBias( layer + 1, target ) );
        }
    }

    // Slack var row: minimal - runnerUp
    unsigned minimalVar = nodeToVars[Index(numLayersInUse - 1, minimal, false)];
    unsigned runnerUpVar = nodeToVars[Index(numLayersInUse - 1, runnerUp, false)];
    reluplex.initializeCell( outputSlackVar, outputSlackVar, -1 );
    reluplex.initializeCell( outputSlackVar, minimalVar, 1 );
    reluplex.initializeCell( outputSlackVar, runnerUpVar, -1 );


    printf("\nrunnerUp Information: ");
    printf("\nminimalIndex: %u, minimalVar: %u ",minimal, minimalVar);
    printf("\nrunnerUpIndex: %u, runnerUpVar: %u ",runnerUp, runnerUpVar);
    printf("\noutputSlackVar: %u \n",outputSlackVar);
    reluplex.printTableauRow(outputSlackVar);

    reluplex.setLogging(false );
    reluplex.setDumpStates(false );
    reluplex.toggleAlmostBrokenReluEliminiation( false );

    timeval start = Time::sampleMicro();
    timeval end;

    bool sat = false;

    //// add by lzs
    double **currentAdversaryE ;
    unsigned num_Node = 10;
    unsigned num_AE = 0;
    unsigned total = 5;
    unsigned num_Expected_AE = 5;

    currentAdversaryE = new double*[total];
    for (unsigned i = 0; i < total; i++) {
        currentAdversaryE[i] = new double[num_Node];
    }

    //// add end


    try
    {


        Vector<double> inputs;
        Vector<double> outputs;

        double totalError = 0.0;

        for ( unsigned i = 0; i < inputLayerSize; ++i ) {
            printf("nodeToVars[Index(0, %u, true)] : %u \n", i, nodeToVars[Index(0, i, true)]);
        }
        for ( unsigned i = 0; i < outputLayerSize; ++i )
        {
            printf("nodeToVars[Index(0, %u, true)] : %u \n", i, nodeToVars[Index(numLayersInUse - 1, i, false)]);

        }


        Reluplex::FinalStatus result = reluplex.solve(currentAdversaryE, num_AE, num_Node, num_Expected_AE);
        if ( result == Reluplex::SAT )
        {
            printf( "\nSolution found!\n\n" );

            for ( unsigned i = 0; i < inputLayerSize; ++i )
            {
                double assignment = reluplex.getAssignment( nodeToVars[Index(0, i, true)] );
                printf( "input[%u] = %lf. Normalized: %lf.\n",
                        i, unnormalizeInput( i, assignment, neuralNetwork ), assignment );
                inputs.append( assignment );
            }

            for ( unsigned i = 0; i < outputLayerSize; ++i )
            {
                printf( "output[%u] = %.10lf. Normalized: %lf\n", i,
                        reluplex.getAssignment( nodeToVars[Index(numLayersInUse - 1, i, false)] ),
                        normalizeOutput( reluplex.getAssignment( nodeToVars[Index(numLayersInUse - 1, i, false)] ),
                                         neuralNetwork ) );
            }

            printf( "\nOutput using nnet.cpp:\n" );
            neuralNetwork.evaluate( inputs, outputs, outputLayerSize );
            unsigned i = 0;
            for ( const auto &output : outputs )
            {
                printf( "output[%u] = %.10lf. Normalized: %lf\n", i, output,
                        normalizeOutput( output, neuralNetwork ) );

                totalError +=
                    FloatUtils::abs( output -
                                     reluplex.getAssignment( nodeToVars[Index(numLayersInUse - 1, i, false)] ) );

                ++i;
            }
            printf( "\n" );
            printf( "Total error: %.10lf. Average: %.10lf\n", totalError, totalError / outputLayerSize );
            printf( "\nReminder: minimal: %u. runner up: %u\n", minimal, runnerUp );

            sat = true;

            //// add by lzs

            printf("\n\n===========================\n");
            printf("Summary!!! printCurrentAE in main:SAT \n");
            for (unsigned j = num_AE; j > 0; --j) {  //倒序输出，为了方便与上面比较，同时unsigned >=0恒为true，所以这里只>0,后面每次使用j时-1即可
                printf("\nThis is a adversial example: %u \n", num_AE - j + 1);
                inputs.clear();
                outputs.clear();
                for (unsigned k = 0; k < num_Node; ++k) {
                    double assignment = currentAdversaryE[j-1][k];
                    if( k < inputLayerSize){ //5
                        printf( "input[%u] = %lf. Normalized: %lf.\n", k, unnormalizeInput( k, assignment, neuralNetwork ), assignment );
                        inputs.append( assignment );

                    } else{
                        printf( "output[%u] = %.10lf. Normalized: %lf\n", k, assignment, normalizeOutput( assignment, neuralNetwork ) );
                    }
                }
                printf( "\nOutput using nnet.cpp:\n" );
                neuralNetwork.evaluate( inputs, outputs, outputLayerSize );
                unsigned b = inputLayerSize;     // 5
                for ( const auto &output : outputs )
                {
                    printf( "output[%u] = %.10lf. Normalized: %lf\n", b, output,
                            normalizeOutput( output, neuralNetwork ) );

                    totalError += FloatUtils::abs( output - currentAdversaryE[j-1][b]);

                    ++b;
                }
                printf("\n-------------------------\n");
            }

            printf( "Total error: %.10lf. \n", totalError );

            //// add end

        }
        else if ( result == Reluplex::UNSAT )
        {
            printf( "Can't solve!\n" );
            sat = false;
        }
        else if ( result == Reluplex::ERROR )
        {
            printf( "Reluplex error!\n" );
            sat = false;
        }else if ( result == Reluplex::NOT_DONE)
        {
            printf( "Reluplex not done (quit called?)\n" );
            sat = false;
        }

        /*** add by lzs ***/
        else{
            sat = true;
            printf("\n----- printStatistics():Reluplex SAT But Then ... ----\n");
            reluplex.printStatistics();

            printf( "Reluplex SAT But Then ...!\n" );

            printf("\n==========================\n");
            printf("Summary!!! printCurrentAE in main: ");  // 后面接上 print_result(),里面有换行符\n
            print_result(result);
//            for (unsigned j = 0; j < num_AE; ++j) {
            for (unsigned j = num_AE; j > 0; --j) {  //倒序输出，为了方便与上面比较，同时unsigned >=0恒为true，所以这里只>0,后面每次使用j时-1即可
                printf("\nThis is a adversial example: %u \n", num_AE - j + 1);
                inputs.clear();
                outputs.clear();
                for (unsigned k = 0; k < num_Node; ++k) {
                    double assignment = currentAdversaryE[j-1][k];
                    if( k < inputLayerSize){ //5
                        printf( "input[%u] = %lf. Normalized: %lf.\n", k, unnormalizeInput( k, assignment, neuralNetwork ), assignment );
                        inputs.append( assignment );

                    } else{
                        printf( "output[%u] = %.10lf. Normalized: %lf\n", k, assignment, normalizeOutput( assignment, neuralNetwork ) );
                    }
                }
                printf( "\nOutput using nnet.cpp:\n" );
                neuralNetwork.evaluate( inputs, outputs, outputLayerSize );
                unsigned b = inputLayerSize;     // 5
                for ( const auto &output : outputs )
                {
                    printf( "output[%u] = %.10lf. Normalized: %lf\n", b, output,
                            normalizeOutput( output, neuralNetwork ) );

                    totalError += FloatUtils::abs( output - currentAdversaryE[j-1][b]);

                    ++b;
                }
                printf("\n-------------------------\n");
            }

            printf( "Total error: %.10lf. \n", totalError );
        }
        /*** add end ***/


        printf( "Number of explored states: %u\n", reluplex.numStatesExplored() );
    }
    catch ( const Error &e )
    {
        printf( "main.cpp: Error caught. Code: %u. Errno: %i. Message: %s\n",
                e.code(),
                e.getErrno(),
                e.userMessage() );
        fflush( 0 );
    }

    end = Time::sampleMicro();

    unsigned milliPassed = Time::timePassed( start, end );
    unsigned seconds = milliPassed / 1000;
    unsigned minutes = seconds / 60;
    unsigned hours = minutes / 60;

    printf( "Total run time: %u milli (%02u:%02u:%02u)\n",
            Time::timePassed( start, end ), hours, minutes - ( hours * 60 ), seconds - ( minutes * 60 ) );

	return sat;
}

int main( int argc, char **argv )
{
    String networkPath = FULL_NET_PATH;
    char *finalOutputFile;

    if ( argc < 2 )
        finalOutputFile = NULL;
    else
        finalOutputFile = argv[1];

//    printf("\nfinalOutputFile:%s\n",finalOutputFile);  // 指向summary.txt文件，只有这一个参数，argv[2]指向null, argv[0]指向./check_properties/bin/adversarial.elf

    //读取网络
    AcasNeuralNetwork neuralNetwork( networkPath.ascii() );

    // 层数、输出层的节点数
    unsigned numLayersInUse = neuralNetwork.getNumLayers() + 1;   //数字上不包含第1层，所以要+1
    printf( "allLayerSize = %u\n", numLayersInUse );

    unsigned outputLayerSize = neuralNetwork.getLayerSize( numLayersInUse - 1 );    //取下标，从0开始，所以最后一层下标为7，要-1
    //输入层的节点数
    unsigned inputLayerSize = neuralNetwork.getLayerSize( 0 );

    // 中间节点ReLU的个数
    unsigned numReluNodes = 0;
    for ( unsigned i = 1; i < numLayersInUse - 1; ++i )  // 下标1 ~ 6 是隐藏层，numLayersInUse - 1 = 7
        numReluNodes += neuralNetwork.getLayerSize( i );

    printf( "Input nodes = %u, relu nodes = %u, output nodes = %u\n", inputLayerSize, numReluNodes, outputLayerSize );

    // 设置输入层节点
    Vector<double> inputs = { 0.2, -0.1, 0, -0.3, 0.4 };
    Vector<double> outputs;

    printf( "\nInputs are: \n" );
    for ( unsigned i = 0; i < inputs.size(); ++i )
    {
        printf( "\tinput[%u] = %lf\n", i, inputs[i] );
    }

    printf( "\nOutput using nnet.cpp:\n" );
    neuralNetwork.evaluate( inputs, outputs, outputLayerSize );
    unsigned i = 0;
    for ( const auto &output : outputs )
    {
        printf( "\toutput[%u] = %.10lf. Normalized: %lf\n", i, output,
                normalizeOutput( output, neuralNetwork ) );
        ++i;
    }


    return 0;
}



//
// Local Variables:
// compile-command: "make -C .. "
// c-basic-offset: 4
// End:
//
