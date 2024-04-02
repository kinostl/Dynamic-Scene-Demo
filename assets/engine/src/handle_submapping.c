#pragma bank 255

#include <gb/gb.h>
#include <rand.h>
#include <asm/types.h>

#include "vm.h"
#include "data_manager.h"
#include "bankdata.h"
#include "compat.h"
#include "gbs_types.h"

#define C_MASK (limit | (limit >> 1) | (limit >> 2) | (limit >> 3) | (limit >> 4) | (limit >> 5) | (limit >> 6) | (limit >> 7) | (limit >> 8) | (limit >> 9) | (limit >> 10) | (limit >> 11) | (limit >> 12) | (limit >> 13) | (limit >> 14) | (limit >> 15))

uint16_t random(uint16_t limit){

    uint16_t value = randw() & C_MASK;
    if (value >= limit) value -= limit;
    if (value >= limit) value -= limit;

    return value;
}


void make_maze(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;
    unsigned char map_buffer[18][20];
    unsigned char coll_buffer[18][20];

    for(int y=0;y<18;y++){
        for(int x=0;x<20;x++){
            map_buffer[y][x] = 0x03;
            coll_buffer[y][x] = 0x0F;
        }
    }


    for(int x=0;x<20;x++){
        map_buffer[0][x] = 0x00;
        coll_buffer[0][x] = 0x00;
    }

    for(int y = 2; y<18; y+=2){
        for(int i = 0; i<20; i++){
            int width = random(10 - (i/2)) * 2;
            int next = i + width + 1;
            if(next>=18) next = 20;

            for(int x=i;x<next;x++){
                map_buffer[y][x] = 0x00;
                coll_buffer[y][x] = 0x00;
            }

            int n_hole = random(width/2) * 2;
            map_buffer[y-1][next - n_hole - 1] = 0x00;
            coll_buffer[y-1][next - n_hole - 1] = 0x00;

            if(random(2) == 1){
                int p_hole = random(width/2) * 2;
                map_buffer[y][next - p_hole - 1] = 0x01;
                coll_buffer[y][next - p_hole - 1] = 0x00;
            }

            i = next;
        }
    }

    scene_t my_scene;
    background_t my_bg;

    far_ptr_t my_far_colls;
    far_ptr_t my_far_bg;
    far_ptr_t my_far_map;

    MemcpyBanked(
        &my_scene,
        current_scene.ptr,
        sizeof(my_scene),
        current_scene.bank
    );

    MemcpyBanked(
        &my_bg,
        my_scene.background.ptr,
        sizeof(my_bg),
        my_scene.background.bank
    );

    my_far_colls.ptr = &coll_buffer;
    my_far_colls.bank = CURRENT_BANK;

    my_far_map.ptr = &map_buffer;
    my_far_map.bank = CURRENT_BANK;

    my_far_bg.ptr = &my_bg;
    my_far_bg.bank = CURRENT_BANK;

    my_bg.tilemap = my_far_map;
    //my_scene -> collisions = my_far_ptr;
    my_scene.background = my_far_bg;
    my_scene.collisions = my_far_colls;

    load_scene(&my_scene, CURRENT_BANK, false);


}
