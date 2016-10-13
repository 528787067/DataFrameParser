#include <stdlib.h>
#include "parser.h"

/**
 * 初始化一个队列
 *
 * @_capacity: 队列总容量
 */
Queue *init_queue(uint8 _capacity)
{
	Queue *queue = (Queue *)malloc(sizeof(Queue));
	queue->capacity = _capacity;
	queue->size = 0;
	return queue;
}

/**
 * 数据入队
 *
 * @_queue: 队列
 * @_data: 数据
 **/
uint8 en_queue(Queue *_queue, uint8 _data)
{
	if(_queue->size < _queue->capacity)
	{
		Node *node = (Node *)malloc(sizeof(Node));
		node->data = _data;
		node->next_node = NULL;

        if(_queue->size == 0)
        {
            node->pre_node = NULL;
            _queue->back = node;
            _queue->front = _queue->back;
        }
        else
        {
            node->pre_node = _queue->back;

            _queue->back->next_node = node;
            _queue->back = _queue->back->next_node;
        }
		_queue->size++;
	}
	else
	{
		Node *temp_node = _queue->front->next_node;
		_queue->front->pre_node = _queue->back;
		_queue->back->next_node = _queue->front;
		_queue->back = _queue->back->next_node;
		_queue->back->data = _data;
		_queue->back->next_node = NULL;
		_queue->front = temp_node;
	}
	return _queue->size-1;
}

/**
 * 数据出队
 *
 * @_queue: 队列
 *
 * @return: 出队的数据
 */
uint8 de_queue(Queue *_queue)
{
    uint8 old_data = 0;

    if(_queue->size > 0)
    {
        old_data = _queue->front->data;
        if(_queue->size == 1)
        {
            free(_queue->front);
            _queue->front = NULL;
            _queue->back = NULL;
        }
        else
        {
            _queue->front = _queue->front->next_node;
            free(_queue->front->pre_node);
            _queue->front->pre_node = NULL;
        }
        _queue->size--;
    }
    return old_data;
}

/**
 * 清空队列
 *
 * @_queue: 队列
 */
void clear_queue(Queue *_queue)
{
    while(_queue->size > 0)
    {
        de_queue(_queue);
    }
}

/**
 * 释放队列
 *
 * @_queue: 队列
 */
void release_queue(Queue *_queue)
{
    clear_queue(_queue);
    free(_queue);
    _queue = NULL;
}
