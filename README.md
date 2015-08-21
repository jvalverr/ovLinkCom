# ovLinkCom
Copyright (C) 2015 by Jorge C. Valverde-Rebaza
Email: jvalverr@icmc.usp.br
Site: http://www.labic.icmc.usp.br/jvalverr/

About
=====

ovLinkCom is a software to calculate communities as groups of links rather than nodes. We consider that each node belongs to all communities formed by links where that node belongs. Thus, is possible obtain overlapping node groups. ovLinkCom is an alternative C++ implementation of hierarchical link communities in complex networks, published by Ahn et al [1]_ . Of course, ovLinkCom is easier and faster than the `original C++ implementation`_. ovLinkCom does not need to do multiple reads of the input file, thus is faster. Also, automatically converts any network format in the appropriate ovLinkCom format without the need for any additional software. The indices of nodes in output maintain correspondence with the indices of nodes from input file. Thus, ovLinkCom is easier.


.. _original C++ implementation: https://github.com/bagrow/linkcomm/tree/master/cpp

Usage
=====

Should be self-explanatory, just run


References
==========

.. [1] Ahn YY, Bagrow JP and Lehmann S: Link communities reveal multiscale complexity in networks. *Nature* **466**, 761 (2010).
