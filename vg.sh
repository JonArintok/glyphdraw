#!/bin/bash
valgrind --suppressions=vg.supp --leak-check=full ./ShaderPunk
