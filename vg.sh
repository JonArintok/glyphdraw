#!/bin/bash
valgrind --suppressions=vg.supp --leak-check=full --gen-suppressions=all ./ShaderPunk
