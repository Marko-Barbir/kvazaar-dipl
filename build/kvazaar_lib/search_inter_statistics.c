#include "search_inter_statistics.h"
#include "Windows.h"

CRITICAL_SECTION cs;

search_inter_statistic_list_t* create_head()
{
  InitializeCriticalSection(&cs);

  search_inter_statistic_list_t *head = malloc(sizeof(search_inter_statistic_list_t));

  head->first_stat = NULL;
  head->last_stat = NULL;

  return head;
}

search_inter_statistic_t* append_next(search_inter_statistic_list_t* stat_list)
{
  EnterCriticalSection(&cs);

  search_inter_statistic_t *next = malloc(sizeof(search_inter_statistic_t));
  next->next_stat = NULL;
  if (stat_list->first_stat == NULL) {
    stat_list->first_stat = next;
  }
  else {
    stat_list->last_stat->next_stat = next;
  }

  stat_list->last_stat = next;

  LeaveCriticalSection(&cs);
  return next;
}

int statistics_list_exit_critical_section() {

  return 0;
}

static void writeHeader(FILE *out_file) {
  fprintf(out_file, "cur_idx,ref_idx,origin_x,origin_y,width,height,pitch,roll,throttle,yaw,direction,start_mv_x,start_mv_y,best_mv_x,best_mv_y,best_cost,best_bits\n");
}

static void writeLine(const search_inter_statistic_t *line, FILE *out_file) {
  fprintf(out_file, "%d,%d,%d,%d,%d,%.5lf,%.5lf,%.5lf,%.5lf,%d,%d,%d,%d,%d,%.5lf,%.5lf\n",
    line->cur_idx, line->origin.x, line->origin.y, line->width, line->height, line->pitch, line->roll, line->throttle, line->yaw,
    line->direction, line->start_mv.x, line->start_mv.y, line->best_mv.x, line->best_mv.y, line->best_cost, line->best_bits);
}

int write_statistics_file(const search_inter_statistic_list_t *stat_list, FILE *out_file)
{
  writeHeader(out_file);

  search_inter_statistic_t *cur = stat_list->first_stat;
  while (cur != NULL) {
    writeLine(cur, out_file);
    cur = cur->next_stat;
   }

  return 0;
}

int free_statistics_list(search_inter_statistic_list_t *stat_list)
{
  DeleteCriticalSection(&cs);

  search_inter_statistic_t *cur = stat_list->first_stat;
  search_inter_statistic_t *tmp;
  while (cur != NULL) {
    tmp = cur;
    cur = cur->next_stat;
    FREE_POINTER(tmp);
  }

  FREE_POINTER(stat_list);

  return 0;
}

