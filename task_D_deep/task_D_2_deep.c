void btUpView(tree *root) {
    if (!root) return;

    /* --- Очередь для BFS --- */
    struct QNode {
        tree       *node;
        int         hd;
        struct QNode *next;
    };

    struct QNode *front = NULL, *rear = NULL;

    /* --- Карта hd -> key (динамический массив) --- */
    int   hdArr[1000];
    int   keyArr[1000];
    int   mapSize = 0;

    /* --- Вспомогательные лямбда-функции через макросы --- */
    #define Q_PUSH(n, h) \
        do { \
            struct QNode *_q = (struct QNode*)malloc(sizeof(struct QNode)); \
            _q->node = (n); _q->hd = (h); _q->next = NULL; \
            if (!rear) { front = rear = _q; } \
            else { rear->next = _q; rear = _q; } \
        } while(0)

    #define MAP_HAS(h) ({ \
        int _found = 0; \
        for (int _i = 0; _i < mapSize; _i++) \
            if (hdArr[_i] == (h)) { _found = 1; break; } \
        _found; \
    })

    Q_PUSH(root, 0);

    while (front) {
        /* pop */
        struct QNode *cur = front;
        tree *node = cur->node;
        int   hd   = cur->hd;
        front = front->next;
        if (!front) rear = NULL;
        free(cur);

        /* если hd встречается впервые — запоминаем */
        if (!MAP_HAS(hd)) {
            hdArr[mapSize]  = hd;
            keyArr[mapSize] = node->key;
            mapSize++;
        }

        if (node->left)  Q_PUSH(node->left,  hd - 1);
        if (node->right) Q_PUSH(node->right, hd + 1);
    }

    /* --- Сортировка по hd (пузырёк) --- */
    for (int i = 0; i < mapSize - 1; i++)
        for (int j = 0; j < mapSize - 1 - i; j++)
            if (hdArr[j] > hdArr[j+1]) {
                int tmp;
                tmp = hdArr[j];  hdArr[j]  = hdArr[j+1];  hdArr[j+1]  = tmp;
                tmp = keyArr[j]; keyArr[j] = keyArr[j+1]; keyArr[j+1] = tmp;
            }

    /* --- Вывод --- */
    for (int i = 0; i < mapSize; i++) {
        if (i) printf(" ");
        printf("%d", keyArr[i]);
    }
    printf("\n");

    #undef Q_PUSH
    #undef MAP_HAS
}

