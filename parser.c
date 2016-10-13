#include <stdlib.h>
#include "parser.h"

/**
 * 初始化一个解析器
 *
 * @_data_header: 数据头指针
 * @_header_size: 数据头大小
 * @_data_footer: 数据尾指针
 * @_foot_size: 数据尾大小
 * @_data_frame_size: 一帧完整数据的大小
 *
 * @return: 解析器
 */
DataParser *parser_init(uint8 *_data_header, uint8 _header_size, uint8 *_data_footer, uint8 _foot_size, uint8 _data_frame_size)
{
    if((_header_size+_foot_size) > _data_frame_size || (_header_size+_foot_size) == 0)
        return NULL;

    DataParser *parser = (DataParser *)malloc(sizeof(DataParser));
    parser->parser_queue = init_queue(_data_frame_size);
    parser->resule_pointer = NULL;
    parser->data_header = _data_header;
    parser->header_size = _header_size;
	parser->data_footer = _data_footer;
	parser->footer_size = _foot_size;
    parser->result_size = _data_frame_size - parser->header_size - parser->footer_size;
    parser->parserResult = RESULT_FALSE;

    while(_data_frame_size-- > 0)
    {
        en_queue(parser->parser_queue, 0);
    }

    return parser;
}

/**
 * 将数据添加到解析器中进行解析
 *
 * @_parser: 解析器
 * @_data: 要解析的数据
 *
 * @return: 当前解析结果，返回 RESULT_TRUE 代表成功解析出一帧数据
 */
ParserResult parser_put_data(DataParser *_parser, uint8 _data)
{
    uint8 i;
    Node *node;

	if(_parser == NULL)
		return RESULT_FALSE;

    en_queue(_parser->parser_queue, _data);

	/* 校验数据尾 */
	node = _parser->parser_queue->back;
	for(i = _parser->footer_size; i > 0; i--)
	{
		if(node->data != _parser->data_footer[i-1])
            goto DATA_FRAME_FALSE;
        node = node->pre_node;
	}

	/* 校验数据头 */
    node = _parser->parser_queue->front;
    for(i = 0; i < _parser->header_size; i++)
    {
        if(node->data != _parser->data_header[i])
            goto DATA_FRAME_FALSE;
        node = node->next_node;
    }

    if(_parser->resule_pointer == NULL && _parser->result_size > 0)
        _parser->resule_pointer = node;
    if(_parser->parserResult != RESULT_TRUE)
    	_parser->parserResult = RESULT_TRUE;
    return _parser->parserResult;

DATA_FRAME_FALSE:
    if(_parser->resule_pointer != NULL)
        _parser->resule_pointer = NULL;
    if(_parser->parserResult != RESULT_FALSE)
        _parser->parserResult = RESULT_FALSE;
    return _parser->parserResult;

}

/**
 * 解析成功后从解析器中取出解析结果
 *
 * @_parser: 解析器
 * @_index: 解析结果集合中的第 _index 个数据
 *
 * @return: 获取解析成功的数据，返回 -1 代表数据获取失败
 */
int parser_get_data(DataParser *_parser, uint8 _index)
{
    Node *node;
    if(_parser == NULL
	|| _parser->parserResult != RESULT_TRUE
    || _index >= _parser->result_size
    || _parser->resule_pointer == NULL)
        return -1;
    node = _parser->resule_pointer;
    while(_index > 0)
    {
        node = node->next_node;
        _index--;
    }
    return node->data;
}

/**
 * 重置解析器
 *
 * @_parser: 解析器
 */
void parser_reset(DataParser *_parser)
{
	uint8 _data_frame_size;

	if(_parser == NULL)
		return;

	_data_frame_size = _parser->parser_queue->size;
	while(_data_frame_size-- > 0)
    {
        en_queue(_parser->parser_queue, 0);
    }
    _parser->resule_pointer = NULL;
    _parser->parserResult = RESULT_FALSE;
}

/**
 * 释放解析器
 *
 * @_parser: 解析器
 */
void parser_release(DataParser *_parser)
{
	if(_parser == NULL)
		return;
    release_queue(_parser->parser_queue);
    free(_parser);
    _parser = NULL;
}
