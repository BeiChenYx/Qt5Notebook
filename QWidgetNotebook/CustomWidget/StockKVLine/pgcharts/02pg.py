import pdb
import pyqtgraph as pg
import numpy as np
import pandas as pd


def read_tick_data():
    """
    生成蜡烛图的数据
    蜡烛图需要 open, high, low, close,timestamp
    从csv文件中获取数据
    """
    tick_data = pd.read_csv('../ts_code_000001_sz_202003010807.csv', encoding='utf-8')
    tick_data['trade_date'] = tick_data['trade_date'].apply(lambda x: str(x))
    return tick_data
