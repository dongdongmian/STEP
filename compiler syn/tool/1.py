#!/usr/bin/env python2
# -*- coding:utf-8 -*-

from bs4 import BeautifulSoup
import re, codecs

if __name__ == '__main__':
    with codecs.open('1.html', 'r', encoding='gbk') as fr:
        html_doc = fr.read()
        
    table = BeautifulSoup(html_doc).find('table')
    tr_list = table.find_all('tr')
    
    first_tr = tr_list.pop(0)
    # 终结符个数
    n_term = int(first_tr.find('p', text='ACTION').parent['colspan'])
    # 非终结符个数
    n_nterm = int(first_tr.find('p', text='GOTO').parent['colspan'])
    
    second_tr = tr_list.pop(0)
    row = [td.text for td in second_tr.find_all('td')]
    # 终结符列表和非终结符列表
    term_list, nterm_list = row[:n_term], row[n_term:]
    # 状态个数
    n_state = len(tr_list)

    action_table = []
    goto_table = []
    for tr in tr_list:
        row = [td.text for td in tr.find_all('td')]
        # 去掉状态
        row.pop(0)
        # 将空白单元格替换为'error'（空白符在unicode里面是'\xa0'，如果只有空白符的地方，为error）
        row = ['error' if elem == u'\xa0' else elem for elem in row]
        # 将unicode符号u'\xa0'替换为':'， 便于之后的C语言处理，便于确认右部变量的个数，同时保存在结构体中
        row = [re.sub(u'\xa0', ':', elem) for elem in row]
        action_table.append(row[:n_term])
        goto_table.append(row[n_term:])

    # 将解析结果写入文件，之后用C语言读取
    fw = codecs.open('1.txt', 'w', encoding='utf8')
    fw.write('n_state:%d n_term:%d n_nterm:%d' % (n_state, n_term, n_nterm)
             + '\n')
    fw.write('\t'.join(term_list) + '\n')
    for row in action_table:
        fw.write('\t'.join(row) + '\n')
    fw.write('\t'.join(nterm_list) + '\n')
    for row in goto_table:
        fw.write('\t'.join(row) + '\n')
