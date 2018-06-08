#!/bin/sh

export LD_LIBRARY_PATH=/home/snetwork/xservice
(nohup /home/snetwork/marketsolutiongateway/marketsolutiongateway5001 --flagfile=./marketsolution5001.cfg &)
