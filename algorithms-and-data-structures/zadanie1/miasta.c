#include <stdio.h>

int main(){
    int n, m, currentid, start, back, p, testid, oldid, col;
    int citycount = 0;
    scanf("%d %d\n", &n, &m);
    char line[m];
    int cityid[2][m];
    for(int k=0; k < 2; k++){
        for(int i=0; i < m; i++){
            cityid[k][i] = 0;
        }
    }
    int id = 1;
    unsigned char curr = 0;
    unsigned char prev = 1;
    for(int k=0; k < n; k++){
        fgets(line, m+2, stdin);
        currentid = 0;
        col = 0;
        oldid = 0;
        start = -2;
        back = -1;
        p = -1;
        for(int i=0; i < m; i++){
            switch(line[i]){
                // Empty cell
                // set current id to 0
                // set old id to 0
                case 'A':
                    currentid = 0;
                    oldid = 0;
                    col = 0;
                    cityid[curr][i] = oldid;
                    start = -2;
                    back = -1;
                    p = i;
                    break;

                // Left Down
                // Check if it got passed an id. End id passing
                // Give new id and indicate that it goes down
                // Increase city count
                // End old id
                case 'B':
                    p = i;
                    if(currentid > 0){
                        cityid[curr][i] = oldid;
                        cityid[curr][i] ^= 0x00000001;
                    } 
                    // Generate new id dont propagate
                    else {
                        cityid[curr][i] = id << 1;
                        cityid[curr][i] ^= 0x00000001;
                        citycount++;
                        id++;
                    }
                    currentid = 0;
                    col = 0;
                    oldid = 0;
                    start = -2;
                    back = -1;
                    break;
                
                // Left Up
                case 'C': 
                    if(currentid == 0){
                        // Get id from upper layer but dont propagate
                        if(cityid[prev][i] % 2 == 1){
                            cityid[curr][i] = cityid[prev][i];
                            cityid[curr][i] ^= 0x00000001;
                        } 
                        // Generate new id and increase city count dont propagate
                        else {
                            cityid[curr][i] = id << 1;
                            id++;
                            citycount++;
                        }
                        p = i;
                        col = 0;

                    } else {
                        // Detect collision and backpropagate to correct previous ids
                        testid = currentid ^ 0x00000001;
                        if(cityid[prev][i] % 2 == 1 && cityid[prev][i] != testid){

                            currentid = cityid[prev][i] ^ 0x00000001;    
                            if(col != 1){
                                col = 1;
                                oldid = currentid;
                                citycount--;
                                p = i;
                                back = i;
                                i = start;
                            } else {
                                citycount--;
                                cityid[curr][i] = oldid;
                            }

                        } 
                        // There was no collision or there was a correction, take old and stop propagating
                        else {
                            p = i;
                            cityid[curr][i] = oldid;
                            currentid = 0;
                            oldid = 0;
                            col = 0;
                        }
                    }
                    start = -2;
                    break;

                //Right Up
                case 'D':
                    // Note: Can't backpropagate from D
                    // If currentid > 0 then this cell needs correction
                    start = i-1;
                    // Stop propagation from previous cell
                    if(p < i){
                        currentid = 0;
                        col = 0;
                        oldid = 0;
                    }
                    p = i;

                    if(currentid > 0){
                        cityid[curr][i] = oldid;
                    } else {
                    // Take id from upper layer and propagate
                        if(cityid[prev][i] % 2 == 1){
                            currentid = cityid[prev][i] ^ 0x00000001;
                            oldid = currentid;
                            cityid[curr][i] = oldid;
                        }
                    // Generate new id
                        else {
                            currentid = id << 1;
                            cityid[curr][i] = currentid;
                            oldid = currentid;
                            id++;
                            citycount++;
                        }
                    }
                    break;

                // Right Down
                case 'E':
                    start = i-1;
                    if(p < i){
                        currentid = 0;
                        oldid = 0;
                        col = 0;
                    }
                    p = i;
                    // Detect currentid from backpropagation
                    if(currentid > 0){
                        cityid[curr][i] = oldid;
                        cityid[curr][i] ^= 0x00000001;
                    }
                    // Generate new id
                    else {
                        currentid = id << 1;
                        cityid[curr][i] = currentid ^ 0x00000001;
                        oldid = currentid;
                        id++;
                        citycount++;
                    }
                    break;


                // Right Up Left Down
                case 'F':
                    if(start < -1){
                        start = i-1;
                    }
                    if(back >= i){
                        cityid[curr][i] = oldid ^ 0x00000001;
                        p = i;
                        break;
                    }

                    if(currentid == 0){

                        // Get id from upper layer and propagate
                        if(cityid[prev][i] % 2 == 1){
                            cityid[curr][i] = cityid[prev][i];
                            currentid = cityid[prev][i] ^ 0x00000001;
                            oldid =  currentid;
                        }
                        // Generate new id
                        else {
                            currentid = id << 1;
                            oldid = currentid;
                            cityid[curr][i] = currentid ^ 0x00000001;
                            id++;
                            citycount++;
                        }
                        col = 0;
                        p = i;
                    } else {
                        testid = currentid ^ 0x00000001;
                        // Check for collision if not backpropagated
                        if(cityid[prev][i] % 2 == 1 && p < i){
                            // Take corrected id
                            p = i;
                            if(cityid[prev][i] == testid){
                                cityid[curr][i] = oldid ^ 0x00000001;
                            } 
                            // Need to backpropagate and set citycount down
                            else {
                                currentid = cityid[prev][i] ^ 0x00000001;
                                if(col != 1){
                                    col = 1;
                                    oldid = currentid;
                                    back = i;
                                    i = start;
                                    citycount--;
                                }
                                else {
                                    cityid[curr][i] = oldid;
                                    citycount--;
                                }


                            }
                        } 
                        // Take propagated id
                        else {
                            cityid[curr][i] = oldid ^ 0x00000001;
                            p=i;
                        }   
                    }
                    break;
            }
        }
        prev = curr;
        curr = prev ^ 0x01;
    }
    //printf("%d %d", n, m);
    //for(int i=0; i < n; i++){
    //    for(int j=0; j < m; j++){
    //      printf("%d\n", cityid[i][j]);
    //  }
    //}
    printf("%d", citycount);
    return 0;
}