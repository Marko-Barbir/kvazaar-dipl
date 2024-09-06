#ifndef SEARCH_INTER_STATISTICS_H_
#define SEARCH_INTER_STATISTICS_H_

#include <global.h>
#include <stdio.h>

typedef struct {
  int x;
  int y;
} point_t;

typedef struct search_inter_statistic_t {
  int32_t cur_idx;
  int32_t ref_idx;

  /**
   * \brief Top-left corner of the PU
   */
  point_t origin;
  point_t offset;
  int32_t width;
  int32_t height;

  double pitch; // RSV
  double roll; // RSH
  double throttle; // LSV
  double yaw; // LSH
  int direction;

  point_t start_mv;
  point_t best_mv;
  double best_cost;
  double best_bits;

  struct search_inter_statistic_t *next_stat;
} search_inter_statistic_t;

typedef struct {
  search_inter_statistic_t *first_stat;
  search_inter_statistic_t *last_stat;
} search_inter_statistic_list_t;

search_inter_statistic_list_t* create_head();

search_inter_statistic_t* append_next(search_inter_statistic_list_t *stat_list);

int statistics_list_exit_critical_section();

int write_statistics_file(const search_inter_statistic_list_t *stat_list, FILE *out_file);

int free_statistics_list(search_inter_statistic_list_t *stat_list);

#endif