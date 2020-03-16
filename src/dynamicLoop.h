//
// Created by tobias on 15.02.20.
//
#include "DataSource.h"
#include "DecompositionMethod.h"

#ifndef DBGTTHESIS_DYNAMICLOOP_H
#define DBGTTHESIS_DYNAMICLOOP_H

/**
 * main loop of a dynamic run
 * @param data data source
 * @param decompositionMethod decomposition method containing an algorithm for the repartitioning
 * @param migrationNr the number of migrations will be inserted here
 * @param communicationNr the number of communications between nodes mapped to different servers will be inserted here
 * @param output_frequency specifies how often information about the run should be printed, higher means less frequent
 * @param invalid_matching_count the number of times an invalid matching is encountered is inserted here, only if debug is enabled
 * @param clusterNr number of clusters
 * @param clusterSize size of each cluster
 * @param augmentation augmentation
 */
void
performDynamicRun(DataSource *data, DecompositionMethod *decompositionMethod, int &migrationNr, int &communicationNr,
                  const int output_frequency, unsigned int &invalid_matching_count, const unsigned int clusterNr,
                  const int clusterSize, const double augmentation) {
    int count = 0;
    while (data->hasNext()) {
        if (count % output_frequency == 0) {
#ifdef USE_DEBUG
            if (!decomposition_tree::check_mapping(decompositionMethod->getMapping(),
                                                   clusterNr,
                                                   clusterSize * augmentation)) {
                invalid_matching_count++;
            }
#endif
            std::cout << "Iteration " + std::to_string(count) << std::endl;
            //data->printInfo();
#ifdef COLLECT_STATS
            decompositionMethod->printInfo();
#endif
        }
        count++;

        auto oldMapping = decompositionMethod->getMapping();

        auto e = data->getNext();
        decompositionMethod->insertAndUpdate(e.getStart(), e.getEnd());

        auto newMapping = decompositionMethod->getMapping();

        if (newMapping[e.getStart()] != newMapping[e.getEnd()]) {
            ++communicationNr;
        }
        for (unsigned int i = 0; i < oldMapping.size(); i++) {
            if (oldMapping[i] != newMapping[i]) {
                migrationNr++;
            }
        }
    }
    decompositionMethod->printInfo();
    std::cout << "Iteration " + std::to_string(count) + " completed." << std::endl;
}

#endif //DBGTTHESIS_DYNAMICLOOP_H
