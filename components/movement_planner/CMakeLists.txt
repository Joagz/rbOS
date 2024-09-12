idf_component_register(
    SRCS "movement_planner.c"
    "darr/darr.c"
    "linkedlist/linkedlist.c"
    "pqueue/pqueue.c"
    "queue/queue.c"
    "algebra/algebra.c"

    INCLUDE_DIRS "."
    "./darr"
    "./linkedlist"
    "./pqueue"
    "./queue"
    "./algebra")
