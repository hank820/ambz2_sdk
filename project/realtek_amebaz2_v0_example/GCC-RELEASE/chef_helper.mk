include project_include.mk

.PHONY: all
all:
	python3 ameba_zap_cluster_list.py --zap_file ../../../third_party/connectedhomeip/examples/chef/devices/$(SAMPLE_NAME).zap
