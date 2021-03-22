#include <cmath>

#include "GaussFilter.h"

GaussFilter::GaussFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

void GaussFilter::do_filter() {
  { wait(); }
  int x, y, a, b;
  while (true) {
    unsigned char r_buf[256][256];
    unsigned char g_buf[256][256];
    unsigned char b_buf[256][256];
    unsigned int cnt = 0;
    for (y = 0; y != 256 ; ++y) {
      for (x = 0; x != 256; ++x) {
        r_buf[x][y] = i_r.read();
        g_buf[x][y] = i_g.read();
        b_buf[x][y] = i_b.read();
        wait();
      }
    } 
    for (b = 0; b != 256; ++b) {
      for (a = 0; a != 256; ++a) {
        if ((a >= 1) && (b >= 1) && (a <= 254) && (b <= 254)) {
          r_val = r_buf[a - 1][b - 1] * mask[0][0] + r_buf[a][b - 1] * mask[1][0] + r_buf[a + 1][b - 1] * mask[2][0]
                + r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1]
                + r_buf[a - 1][b + 1] * mask[0][2] + r_buf[a][b + 1] * mask[1][2] + r_buf[a + 1][b + 1] * mask[2][2];
          g_val = g_buf[a - 1][b - 1] * mask[0][0] + g_buf[a][b - 1] * mask[1][0] + g_buf[a + 1][b - 1] * mask[2][0]
                + g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1]
                + g_buf[a - 1][b + 1] * mask[0][2] + g_buf[a][b + 1] * mask[1][2] + g_buf[a + 1][b + 1] * mask[2][2]; 
          b_val = b_buf[a - 1][b - 1] * mask[0][0] + b_buf[a][b - 1] * mask[1][0] + b_buf[a + 1][b - 1] * mask[2][0]
                + b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1]
                + b_buf[a - 1][b + 1] * mask[0][2] + b_buf[a][b + 1] * mask[1][2] + b_buf[a + 1][b + 1] * mask[2][2];                
          cnt = 16;
        } else if ((a == 0) && (b == 0)) {
          r_val = r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1]
                + r_buf[a][b + 1] * mask[1][2] + r_buf[a + 1][b + 1] * mask[2][2];
          g_val = g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1]
                + g_buf[a][b + 1] * mask[1][2] + g_buf[a + 1][b + 1] * mask[2][2]; 
          b_val = b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1]
                + b_buf[a][b + 1] * mask[1][2] + b_buf[a + 1][b + 1] * mask[2][2];                
          cnt = 9;
        } else if ((a == 255) && (b == 0)) {
          r_val = r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1]
                + r_buf[a - 1][b + 1] * mask[0][2] + r_buf[a][b + 1] * mask[1][2];
          g_val = g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1]
                + g_buf[a - 1][b + 1] * mask[0][2] + g_buf[a][b + 1] * mask[1][2]; 
          b_val = b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1]
                + b_buf[a - 1][b + 1] * mask[0][2] + b_buf[a][b + 1] * mask[1][2]; 
          cnt = 9;
        } else if ((a == 0) && (b == 255)) {
          r_val = mask[0][0] + r_buf[a][b - 1] * mask[1][0] + r_buf[a + 1][b - 1] * mask[2][0]
                + mask[0][1] + r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1];
          g_val = mask[0][0] + g_buf[a][b - 1] * mask[1][0] + g_buf[a + 1][b - 1] * mask[2][0]
                + mask[0][1] + g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1]; 
          b_val = mask[0][0] + b_buf[a][b - 1] * mask[1][0] + b_buf[a + 1][b - 1] * mask[2][0]
                + mask[0][1] + b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1];                
          cnt = 9;          
        } else if ((a == 255) && (b == 255)) {
          r_val = r_buf[a - 1][b - 1] * mask[0][0] + r_buf[a][b - 1] * mask[1][0]
                + r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1];
          g_val = g_buf[a - 1][b - 1] * mask[0][0] + g_buf[a][b - 1] * mask[1][0]
                + g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1];
          b_val = b_buf[a - 1][b - 1] * mask[0][0] + b_buf[a][b - 1] * mask[1][0]
                + b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1];
          cnt = 9;          
        } else if (b == 0) {
          r_val = r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1]
                + r_buf[a - 1][b + 1] * mask[0][2] + r_buf[a][b + 1] * mask[1][2] + r_buf[a + 1][b + 1] * mask[2][2];
          g_val = g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1]
                + g_buf[a - 1][b + 1] * mask[0][2] + g_buf[a][b + 1] * mask[1][2] + g_buf[a + 1][b + 1] * mask[2][2]; 
          b_val = b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1]
                + b_buf[a - 1][b + 1] * mask[0][2] + b_buf[a][b + 1] * mask[1][2] + b_buf[a + 1][b + 1] * mask[2][2];                
          cnt = 12;
        } else if (a == 0) {
          r_val = r_buf[a][b - 1] * mask[1][0] + r_buf[a + 1][b - 1] * mask[2][0]
                + r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1]
                + r_buf[a][b + 1] * mask[1][2] + r_buf[a + 1][b + 1] * mask[2][2];
          g_val = g_buf[a][b - 1] * mask[1][0] + g_buf[a + 1][b - 1] * mask[2][0]
                + g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1]
                + g_buf[a][b + 1] * mask[1][2] + g_buf[a + 1][b + 1] * mask[2][2]; 
          b_val = b_buf[a][b - 1] * mask[1][0] + b_buf[a + 1][b - 1] * mask[2][0]
                + b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1]
                + b_buf[a][b + 1] * mask[1][2] + b_buf[a + 1][b + 1] * mask[2][2];                
          cnt = 12;          
        } else if (b == 255) {
          r_val = r_buf[a - 1][b - 1] * mask[0][0] + r_buf[a][b - 1] * mask[1][0] + r_buf[a + 1][b - 1] * mask[2][0]
                + r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1] + r_buf[a + 1][b]     * mask[2][1];
          g_val = g_buf[a - 1][b - 1] * mask[0][0] + g_buf[a][b - 1] * mask[1][0] + g_buf[a + 1][b - 1] * mask[2][0]
                + g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1] + g_buf[a + 1][b]     * mask[2][1];
          b_val = b_buf[a - 1][b - 1] * mask[0][0] + b_buf[a][b - 1] * mask[1][0] + b_buf[a + 1][b - 1] * mask[2][0]
                + b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1] + b_buf[a + 1][b]     * mask[2][1];
          cnt = 12;
        } else if (a == 255) {
          r_val = r_buf[a - 1][b - 1] * mask[0][0] + r_buf[a][b - 1] * mask[1][0]
                + r_buf[a - 1][b]     * mask[0][1] + r_buf[a][b]     * mask[1][1]
                + r_buf[a - 1][b + 1] * mask[0][2] + r_buf[a][b + 1] * mask[1][2];
          g_val = g_buf[a - 1][b - 1] * mask[0][0] + g_buf[a][b - 1] * mask[1][0]
                + g_buf[a - 1][b]     * mask[0][1] + g_buf[a][b]     * mask[1][1]
                + g_buf[a - 1][b + 1] * mask[0][2] + g_buf[a][b + 1] * mask[1][2]; 
          b_val = b_buf[a - 1][b - 1] * mask[0][0] + b_buf[a][b - 1] * mask[1][0]
                + b_buf[a - 1][b]     * mask[0][1] + b_buf[a][b]     * mask[1][1]
                + b_buf[a - 1][b + 1] * mask[0][2] + b_buf[a][b + 1] * mask[1][2];                
          cnt = 12;          
        } else;
//        wait();
        o_result_r.write(r_val / cnt);    
        o_result_g.write(g_val / cnt);
        o_result_b.write(b_val / cnt);
      }
    }      
  }
}
