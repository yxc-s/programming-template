
/*
 * Mo(莫队算法)
 * 算法思想：对于离线的区间查询，我们设立两个指针代表区间左右端点，并对查询的区间按照一定的规则分块，并排序，使这两个指针移动的曼哈顿距离最小。
 * 
 * 使用注意事项：BLOCK_SIZE是一个模板参数，大小是数据量大小的开平方。
 *              区间的排序规则已经设定好，右端点的移动是一个波浪状，升降升降升.... and so on.
 *              左指针和右指针的初值应该为1和0。
 *              查询离线处理好后，可以直接对查询进行暴力搜索。
 * 
 * 使用方法：直接将查询放到结构体中进行排序后暴力查询即可。
 * 
 * gitHub(仓库地址): https://github.com/yxc-s/programming-template.git
 */









template<const unsigned int BLOCK_SIZE>
struct Query{
    unsigned int left;
    unsigned int right;
    unsigned int query_id;
    unsigned int block_id;
    long long ans;

    Query(){}

    Query(unsigned int l_, unsigned int r_, unsigned int id_):
        left(l_),
        right(r_),
        query_id(id_),
        block_id(l / BLOCK_SIZE)
    {}

    friend bool operator < (const Query& lhs, const Query& rhs) {
        return lhs.block_id != rhs.block_id ? lhs.block_id < rhs.block_id :
            lhs.block_id & 1 ? lhs.right > rhs.right : lhs.right < rhs.right;
    }
};