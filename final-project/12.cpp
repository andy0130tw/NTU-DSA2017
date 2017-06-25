#include <cstdio>
#include <algorithm>
#define MAX 100010

struct planet {
    char* name;
    int* coord;
};

struct kdNode {
    // -1 means point to a planet of ((planet*) left)
    int dimen;
    int value;
    kdNode* left;
    kdNode* right;
};

struct planetComp {
    planetComp(int d): dimen(d) {};
    bool operator() (const planet* pt1, const planet* pt2){
        return ((*pt1).coord[dimen] < (*pt2).coord[dimen]);
    }
    private: int dimen;
};


int nplanet, nquery, ndimen;

char names[MAX][256];
planet* planetsArr[MAX];

kdNode treePool[MAX * 2];
int treeLen = 0;

// build and return
static kdNode* buildKDTree(int ll, int rr) {
    kdNode* ret = &treePool[treeLen++];
    if (rr - ll == 1) {
        ret->dimen = -1;
        ret->left = (kdNode*) planetsArr[ll];
        return ret;
    }

    // decide which dimension to cut
    int maxDiff = -1, bestDimen = -1, baseVal;
    for (int d = 0; d < ndimen; d++) {
        int dmin = planetsArr[ll]->coord[d];
        int dmax = planetsArr[ll]->coord[d];
        for (int i = ll + 1; i < rr; i++) {
            int cand = planetsArr[i]->coord[d];
            if (cand < dmin) dmin = cand;
            else if (cand > dmax) dmax = cand;
        }
        // printf("split on dimen %d, diff %d (%d-%d)\n", d, dmax-dmin, dmin, dmax);
        if (dmax - dmin > maxDiff) {
            maxDiff = dmax - dmin;
            baseVal = dmin;
            bestDimen = d;
        }
    }

    ret->dimen = bestDimen;
    ret->value = baseVal + maxDiff / 2;

    std::sort(planetsArr + ll, planetsArr + rr, planetComp(bestDimen));
    int mid = ll;
    while (mid < rr && planetsArr[mid]->coord[bestDimen] <= ret->value) {
        mid++;
    }
    // printf("[%d, %d] split from dim %d mid %d, thr %d\n", ll, rr, bestDimen, mid, ret->value);

    ret->left = buildKDTree(ll, mid);
    ret->right  = buildKDTree(mid, rr);
    return ret;
}

static void printTree(kdNode* p) {
    static int dep = 0;
    if (p->dimen >= 0) {
        printf("dimen=%d, value=%d\n", p->dimen, p->value);
        dep += 2;
        for (int i = 0; i < dep; i++) putchar(' ');
        printf("< "); printTree(p->left);
        for (int i = 0; i < dep; i++) putchar(' ');
        printf("> "); printTree(p->right);
        dep -= 2;
    } else {
        auto leaf = (planet*)(p->left);
        printf("node [%s]  %d", leaf->name, leaf->coord[0]);
        for (int i = 1; i < ndimen; i++) {
            printf(",%d", leaf->coord[i]);
        }
        puts("");
    }
}

static inline long long int distSqr(const planet& a, const planet& b) {
    long long int ret = 0ULL;
    for (int d = 0; d < ndimen; d++) {
        long long int diff = a.coord[d] - b.coord[d];
        ret += diff * diff;
    }
    return ret;
}

static planet* findNearest(kdNode* root, planet& pt, long long int* distRef) {
    int dimen = root->dimen;
    if (dimen < 0) {
        planet* tar = (planet*)(root->left);
        long long int distCand = distSqr(pt, *tar);
        // printf("find tar %s, distCand = %lld\n", tar->name, distCand);
        if (distCand < *distRef) {
            *distRef = distCand;
        }
        return tar;
    }
    kdNode* near, * far;
    int overBoundary = 0;
    if (pt.coord[dimen] <= root->value) {
        // printf("go left, distRef = %lld\n", *distRef);
        near = root->left, far = root->right;
        if (pt.coord[dimen] + *distRef > root->value)
            overBoundary = 1;
    } else {
        // printf("go right, distRef = %lld\n", *distRef);
        near = root->right, far = root->left;
        if (pt.coord[dimen] - *distRef <= root->value)
            overBoundary = 1;
    }
    planet* nearest = findNearest(near, pt, distRef);
    if (overBoundary) {
        // printf("find far = %lld\n", *distRef);
        planet* overlap = findNearest(far, pt, distRef);
        if (distSqr(pt, *overlap) < distSqr(pt, *nearest)) {
            nearest = overlap;
        }
    }
    return nearest;
}

int main() {
    scanf("%d%d%d", &nplanet, &nquery, &ndimen);
    planet planets[nplanet];
    int coords[nplanet][ndimen];

    for (int i = 0; i < nplanet; i++) {
        planetsArr[i] = &planets[i];
        planets[i].coord = coords[i];

        scanf("%s", names[i]);
        planets[i].name = names[i];
        for (int d = 0; d < ndimen; d++) {
            scanf("%d", &coords[i][d]);
        }
    }

    kdNode* root = buildKDTree(0, nplanet);

    // printTree(root);

    for (int i = 0; i < nquery; i++) {
        int query[ndimen];
        for (int d = 0; d < ndimen; d++) {
            scanf("%d", &query[d]);
        }
        planet pt { nullptr, query };
        // query
        long long int distRef = 0x7f7f7f7f7f7f7f7fLL;
        planet* result = findNearest(root, pt, &distRef);

        printf("%s\n", result->name);
    }
}
