internal TextualNode*
textual_list_push(Arena *arena, TextualList *list, Textual textual)
{ 
  TextualNode *node = push_array_no_zero(arena, TextualNode, 1);
  SLLQueuePush(list->first, list->last, node);
  list->node_count += 1;
  node->textual = textual;
  return node;
}
