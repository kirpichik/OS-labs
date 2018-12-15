
#ifndef SYNC_PRIM_H
#define SYNC_PRIM_H

void sync_init();

void sync_parent_prelock();

void sync_child_prelock();

void sync_parent_postunlock();

void sync_child_postunlock();

void sync_parent_lock();

void sync_parent_unlock();

void sync_child_lock();

void sync_child_unlock();

void sync_destroy();

#endif
