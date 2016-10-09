ovLinkCom
=====
Copyright (C) 2015-2016 by `Jorge C. Valverde-Rebaza`_

.. _Jorge C. Valverde-Rebaza: http://www.labic.icmc.usp.br/jvalverr/

About
=====

ovLinkCom is a software to calculate communities as groups of links rather than nodes. We consider that each node belongs to all communities formed by links where that node belongs. Thus, is possible obtain overlapping node groups. ovLinkCom is an alternative C++ implementation of hierarchical link communities in complex networks, published by Ahn et al [1]_ . Of course, ovLinkCom is easier and faster than the `original C++ implementation`_ . ovLinkCom does not need to do multiple reads of the input file, thus is faster. Also, automatically converts any network format in the appropriate ovLinkCom format without the need for any additional software. The indices of nodes in output maintain correspondence with the indices of nodes from input file. Thus, ovLinkCom is easier.


.. _original C++ implementation: https://github.com/bagrow/linkcomm/tree/master/cpp

Usage
=====

Should be self-explanatory. First you need get the binary file running::

$ g++ -O5 -o ovLinkCom ovLinkCom.cpp

After that, you can use the binary for any network, is just run::

$ ./ovLinkCom networkFile threshold

Where, ``networkFile`` is an integer links list (one link, two nodes separated by space, per line) and ``threshold`` is the [0,1] threshold for the clustering. 

Output
======

Results are written to different files created automatically. The ``.clusters`` file will contain one cluster of links per line, where link nodes are comma-separated and links are space-separated. The ``.groups`` file will contain also one cluster of links per line, but showing just the nodes forming the links that belong to the cluster. The ``.stats`` file will contain information of each cluster per line, i.e. number of links and number of induced nodes. Finally, the ``.info`` file will contain general information about the overlapping community detection process.

References
==========

.. [1] Ahn YY, Bagrow JP and Lehmann S: Link communities reveal multiscale complexity in networks. *Nature* **466**, 761 (2010).
