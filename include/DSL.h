#ifndef DSL_H
#define DSL_H

#define NTYPE      node->type
#define NDATA      node->data
#define NDTYPE     node->data.additional_type
#define NDNUM      node->data.num

#define LNODE      node->LeftNode
#define RNODE      node->RightNode
#define LNTYPE     node->LeftNode->type
#define RNTYPE     node->RightNode->type
#define LNDATA     node->LeftNode->data
#define RNDATA     node->RightNode->data
#define LNDTYPE    node->LeftNode->data.additional_type
#define RNDTYPE    node->RightNode->data.additional_type
#define LNDNUM     node->LeftNode->data.num
#define RNDNUM     node->RightNode->data.num

#define _DIFF(node) DifferentiatorRec(node, part)
#define _CPY(node)  CopyNode         (node)

#define _VAR(tp)           CreateNode(TpVr, (node_data_t){.additional_type = tp   }, NULL , NULL )
#define _NUM(val)          CreateNode(TpNm, (node_data_t){.num            = val  }, NULL , NULL )
#define _ADD(lnode, rnode) CreateNode(TpOp, (node_data_t){.additional_type = OpAdd}, lnode, rnode)
#define _SUB(lnode, rnode) CreateNode(TpOp, (node_data_t){.additional_type = OpSub}, lnode, rnode)
#define _MUL(lnode, rnode) CreateNode(TpOp, (node_data_t){.additional_type = OpMul}, lnode, rnode)
#define _DIV(lnode, rnode) CreateNode(TpOp, (node_data_t){.additional_type = OpDiv}, lnode, rnode)
#define _POW(lnode, rnode) CreateNode(TpOp, (node_data_t){.additional_type = OpPow}, lnode, rnode)

#define _LN(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnLn  }, node, NULL)
#define _LG(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnLg  }, node, NULL)
#define _LOG(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnLog }, node, NULL)
#define _SIN(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnSin }, node, NULL)
#define _COS(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnCos }, node, NULL)
#define _TG(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnTg  }, node, NULL)
#define _CTG(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnCtg }, node, NULL)
#define _ASIN(node)        CreateNode(TpFn, (node_data_t){.additional_type = FnAsin}, node, NULL)
#define _ACOS(node)        CreateNode(TpFn, (node_data_t){.additional_type = FnAcos}, node, NULL)
#define _ATG(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnTg  }, node, NULL)
#define _ACTG(node)        CreateNode(TpFn, (node_data_t){.additional_type = FnCtg }, node, NULL)
#define _SH(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnSh  }, node, NULL)
#define _CH(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnCh  }, node, NULL)
#define _TH(node)          CreateNode(TpFn, (node_data_t){.additional_type = FnTh  }, node, NULL)
#define _CTH(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnCth }, node, NULL)
#define _SQRT(node)        CreateNode(TpFn, (node_data_t){.additional_type = FnSqrt}, node, NULL)
#define _EXP(node)         CreateNode(TpFn, (node_data_t){.additional_type = FnExp }, node, NULL)

#endif