internal B32
sqlite_open_db(String8 db_path, StateDB *state)
{
  B32 result = 1;
  SQLiteState *sqlite_state = (SQLiteState *)state->memory;
  if(!sqlite_state->api.open_db((char*)db_path.str, &sqlite_state->db) == SQLITE_OK)
  {
    result = 0;
    state->error = DB_Error_Cant_Connect;
  }
  return result;
}

internal B32
sqlite_close_db(StateDB *state)
{
  B32 result = 0;
  SQLiteState *sqlite_state = (SQLiteState *)state->memory;
  if(sqlite_state->api.close_db(sqlite_state->db) == SQLITE_OK)
  {
    sqlite_state->db = 0;
    result = 1;
  }
  return result;
} 

internal B32
sqlite_init(Arena *arena, String8 lib_path, String8 db_path, StateDB *state)
{
  B32 result = 1;
  SQLiteState *sqlite_state = (SQLiteState *)state->memory;
  state->lib = os_library_open(arena, lib_path);
  if(os_handle_match(state->lib, os_handle_zero()))
  {
    state->error = DB_Error_Library;
    printf("Couldn't find the SQLite3 dll");
    return 0;
  }
  SQLiteAPI api = {0};

  api.open_db             = (sqlite_open *)         os_library_load_proc(arena, state->lib, str8_lit("sqlite3_open"));
  api.close_db            = (sqlite_close *)        os_library_load_proc(arena, state->lib, str8_lit("sqlite3_close"));
  api.exec_query          = (sqlite_exec *)         os_library_load_proc(arena, state->lib, str8_lit("sqlite3_exec"));
  api.prepare_query       = (sqlite_prepare *)      os_library_load_proc(arena, state->lib, str8_lit("sqlite3_prepare"));
  api.step_query          = (sqlite_step *)         os_library_load_proc(arena, state->lib, str8_lit("sqlite3_step"));
  api.column_type         = (sqlite_column_type *)  os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_type"));
  api.column_value        = (sqlite_col_value *)    os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_value"));
  api.bind_int            = (sqlite_bind_int *)     os_library_load_proc(arena, state->lib, str8_lit("sqlite3_bind_int"));
  api.column_count        = (sqlite_column_count *) os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_count"));  
  api.column_int          = (sqlite_column_int *)   os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_int"));
  api.column_text         = (sqlite_column_text *)  os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_text"));
  api.column_text16       = (sqlite_column_text16 *)os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_text16"));
  api.column_double       = (sqlite_column_double *)os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_double"));
  api.column_bytes        = (sqlite_column_bytes *) os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_bytes"));
  api.finalize_statement  = (sqlite_finalize *)     os_library_load_proc(arena, state->lib, str8_lit("sqlite3_finalize"));
  api.col_name            = (sqlite_col_name *)     os_library_load_proc(arena, state->lib, str8_lit("sqlite3_column_name"));
  
  if(!api.close_db || !api.open_db || !api.exec_query || 
      !api.prepare_query || !api.step_query || !api.column_type ||
      !api.column_value || !api.bind_int || !api.column_count ||
      !api.column_int || !api.column_text || !api.column_text16 ||
      !api.column_double || !api.column_bytes || !api.finalize_statement)
  {
    state->error = DB_Error_Library;
    printf("Error: sqlite library. Some exported methods was not found.");
    return 0;
  }
  sqlite_state->api = api;
  if(!sqlite_open_db(db_path, state))
  {
    state->is_initialized = 1;
  }
  return result;
} 

internal B32
sqlite_prepare_query(String8 query, StateDB *state)
{
  B32 result = 1;
  SQLiteState *sqlite_state = (SQLiteState *)state;
  if(sqlite_state->api.prepare_query(sqlite_state->db, (char *)query.str, -1, &sqlite_state->statement, NULL)) 
  {
      result = 0;
  }
  return result;
}

internal String8
sqlite_get_column_name(StateDB *state, int col_idx)
{
  String8 result = {0};
  SQLiteState *sqlite_state = (SQLiteState *)state;
  const char * column_name = sqlite_state->api.col_name(sqlite_state->statement, col_idx);
  result = str8_cstring((char *)column_name);
  return result;
}

internal int
sqlite_step_query(StateDB *state)
{
  int result = 0;
  SQLiteState *sqlite_state = (SQLiteState *)state;
  result = sqlite_state->api.step_query(sqlite_state->statement);
  return result;
}

internal ColumnData
sqlite_column_value(Arena *arena, int column_idx, StateDB *state, 
                    Textual column_meaning_table[]) 
{
  ColumnData result = {0};
  SQLiteState *sqlite_state = (SQLiteState *)state;
  result.name    = sqlite_get_column_name(state, column_idx);
  result.textual = database_get_textual(result.name, column_meaning_table);
  
  int colType        = sqlite_state->api.column_type(sqlite_state->statement, column_idx);
  result.byte_count  = sqlite_state->api.column_bytes(sqlite_state->statement, column_idx);
  
  switch(colType) 
  {
    case SQLITE_INTEGER:
    {
      result.type = ColumnType_Text;
      int value = sqlite_state->api.column_int(sqlite_state->statement, column_idx);
      result.value.str = push_array(arena, U8, 10); 
      stbsp_sprintf((char *)result.value.str, "%d", value);
    } break;

    case SQLITE_FLOAT:
    {
      result.type = ColumnType_Float;
      F64 value = (F64)sqlite_state->api.column_int(sqlite_state->statement, column_idx);
      result.value.str = push_array(arena, U8, 20); 
      stbsp_sprintf((char *)result.value.str, "%f", value);
    } break;

    case SQLITE_TEXT:
    {
      result.type = ColumnType_Text;
      const unsigned char *value = sqlite_state->api.column_text(sqlite_state->statement, column_idx);
      String8 text = str8_cstring((char *)value);
      if(result.textual.type == TextType_Title)
      {
        text = str8_from_last_slash(text);
        text = push_str8_copy(arena, str8_chop_last_dot(text));
      }
      result.value.str = push_array(arena, U8, text.size); 
      result.value.str = (U8 *)value;
      result.value.size = text.size;
    } break;
    
    case SQLITE_NULL:
    {
      result.type = TextType_Unmeaningfull;
    } break;
    
    case SQLITE_BLOB:
    {
      result.type = ColumnType_Blob;
    } break;
  }
  AssertAlways(result.type);
  return result;
}

/*
  TODO: should not be here, but because of C/C++ ...
*/
internal ColumnDataNode*
database_column_list_push(Arena *arena, ColumnDataList *list, ColumnData col)
{ 
  ColumnDataNode *node = push_array_no_zero(arena, ColumnDataNode, 1);
  SLLQueuePush(list->first, list->last, node);
  list->node_count += 1;
  node->col = col;
  return node;
}

internal void
sqlite_exec_query_push(Arena *arena, String8 query, StateDB *state,
                            ColumnDataList *list, Textual column_meaning_table[])
{
  SQLiteState *sqlite_state = (SQLiteState *)state;
  if(sqlite_prepare_query(query, state))
  {
    sqlite_state->api.bind_int (sqlite_state->statement, 1, 2);
    U32 column_count = (U32)sqlite_state->api.column_count(sqlite_state->statement);
    
    while(sqlite_step_query(state) != SQLITE_DONE)
    {
      for(U32 column_idx=0; column_idx <= column_count-1; ++column_idx)
      {      
        ColumnData column = sqlite_column_value(arena, column_idx, state, column_meaning_table);
        if(column.textual.type != TextType_Null)
        {
          database_column_list_push(arena, list, column);
        }
      }
    }
    
    if(sqlite_state->api.finalize_statement(sqlite_state->statement) != SQLITE_OK)
    {
      printf("ERROR: Could not finalize statement\n");
      state->error = DB_Error_query_failed;
    }
  }
}