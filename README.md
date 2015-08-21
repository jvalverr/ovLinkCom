# ovLinkCom
This is a software to computes overlapping links communities for large-scale networks.

ovLinkCom is a software to calculate communities as groups of links rather than nodes. We consider that each node belongs to all communities formed by links where that node belongs. Thus, is possible obtain overlapping node groups. This implementation is based on paper [1].

[1] Yong-Yeol Ahn, James P. Bagrow and Sune Lehmann. "Link communities reveal multiscale complexity in networks". Nature 466, pp. 761–764, 2010.

ovLinkCom implementation is based also in the C++ implementation available here: http://barabasilab.neu.edu/projects/linkcommunities/ The main difference of ovLinkCom regarding this implementation is that ovLinCom is easier and faster. ovLinkCom does not need to do multiple reads of the input file, thus is faster. Also, automatically converts any network format in the appropriate ovLinkCom format without the need for any additional software. The indices of nodes in output maintain correspondence with the indices of nodes from input file. Thus, ovLinkCom is easier.

