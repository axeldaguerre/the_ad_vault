internal StateDB*
database_init(Arena *arena, TypeDB type, String8 lib_path, String8 db_path)
{
  StateDB *state = push_array(arena, StateDB, 1);
  state->db_type = type;
  
  B32 is_init = 0;
  if(state->db_type == TypeDB_SQLITE)
  {
     is_init = sqlite_init(arena, lib_path, db_path, state);
  }
  state->is_initialized = is_init;
  return state;
}

internal B32
database_close(StateDB *state)
{
  B32 result = 0;
  if(state->db_type == TypeDB_SQLITE)
  {
    if(sqlite_close_db(state))
    {
      result = 1;
    }
  }
  return result;
}

internal ColumnDataList*
database_exec_query(Arena *arena, String8 query, StateDB *state,
                    TextualTable *textual_table)
{
  ColumnDataList *result = push_array(arena, ColumnDataList, 1);
  if(state->db_type == TypeDB_SQLITE)
  {
    sqlite_exec_query_push(arena, query, state, result, textual_table);
  }
  return result;
}

internal TextualList*
database_textual_from_col(Arena *arena, ColumnDataList *col_list)
{
  TextualList *result = push_array(arena, TextualList, 1);
  for(ColumnDataNode *node = col_list->first; node != 0; node = node->next)
  {
    Textual textual = {0};
    textual_list_push(arena, result, textual);
    
   
  }
  return result;
}  