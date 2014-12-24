__author__ = 'novokreshchenov.konstantin'

import os
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages


FILE_NAME = 'data1.txt'
LOG_FILE = 'client_21.log'
X_LABEL = 'x'
Y_LABEL = 'y'

KEY_GENERATE_REQUEST_START  = 'GENERATE_REQUEST_START'
KEY_GENERATE_REQUEST_END    = 'GENERATE_REQUEST_END'
KEY_CONNECT_TO_SERVER_START = 'CONNECT_TO_SERVER_START'
KEY_CONNECT_TO_SERVER_END   = 'CONNECT_TO_SERVER_END'
KEY_SEND_REQUEST_START      = 'SEND_REQUEST_START'
KEY_SEND_REQUEST_END        = 'SEND_REQUEST_END'
KEY_RECEIVE_RESPONSE_START  = 'RECEIVE_RESPONSE_START'
KEY_RECEIVE_RESPONSE_END    = 'RECEIVE_RESPONSE_END'
KEY_CLOSE_CONNECTION_START  = 'CLOSE_CONNECTION_START'
KEY_CLOSE_CONNECTION_END    = 'CLOSE_CONNECTION_END'

KEY_RESPONSE_STATUS = 'STATUS'
KEY_RESPONSE_RESULT = 'RESULT'

time_keys = [KEY_GENERATE_REQUEST_START, KEY_GENERATE_REQUEST_END,
             KEY_CONNECT_TO_SERVER_START, KEY_CONNECT_TO_SERVER_END,
             KEY_SEND_REQUEST_START, KEY_SEND_REQUEST_END,
             KEY_RECEIVE_RESPONSE_START, KEY_RECEIVE_RESPONSE_END,
             KEY_CLOSE_CONNECTION_START, KEY_CLOSE_CONNECTION_END]

rs_keys = [KEY_RESPONSE_STATUS, KEY_RESPONSE_RESULT]


class ClientReport:
    def __init__(self, d):
        self.time_generating_request = d[KEY_GENERATE_REQUEST_END] - d[KEY_GENERATE_REQUEST_START]
        self.time_connection_to_server = d[KEY_CONNECT_TO_SERVER_END] - d[KEY_CONNECT_TO_SERVER_START]
        self.time_sending_request = d[KEY_SEND_REQUEST_END] - d[KEY_SEND_REQUEST_START]
        self.time_receiving_response = d[KEY_RECEIVE_RESPONSE_END] - d[KEY_RECEIVE_RESPONSE_START]
        self.time_closing_connection = d[KEY_CLOSE_CONNECTION_END] - d[KEY_CLOSE_CONNECTION_START]
        self.status = d[KEY_RESPONSE_STATUS]
        self.result = d[KEY_RESPONSE_RESULT]

        self.time_connecting_closing = self.time_connection_to_server + self.time_closing_connection
        self.time_sending_receiving = self.time_sending_request + self.time_receiving_response
        self.total_time = self.time_generating_request \
                          + self.time_connection_to_server \
                          + self.time_sending_request \
                          + self.time_receiving_response \
                          + self.time_closing_connection


def parse_client_log(log_file):
    d = {}
    llines = [line.strip().split() for line in open(log_file).readlines() if len(line) > 0]
    for lline in llines:
        for time_key in time_keys:
            if time_key in lline:
                d[time_key] = int(lline[lline.index('[') + 1])
        for rs_key in rs_keys:
            if rs_key in lline:
                d[rs_key] = lline[lline.index(rs_key) + 1]

    return ClientReport(d)

# TESTS_DIR
#  |
#  |----> test_cl1_rs1
#          |----> client_1.log
#  | ...
#  |----> test_clM_rsN
#          |----> client_1.log
#          | ...
#          |----> client_M.log
#


class TestsData:
    def __init__(self):
        self.data = {}

    def add_client_reports(self, clients_count, request_size, client_reports):
        if not clients_count in self.data:
            self.data[clients_count] = {}
        self.data[clients_count][request_size] = client_reports


def parse_test_dir(tests_dir):
    tests_data = TestsData()

    for _, test_dirs, _ in os.walk(tests_dir):
        for test_dir_name in test_dirs:
            _, clients_label, requests_label = test_dir_name.split('_')
            clients_count = int(clients_label[2:])
            if clients_count > 30:
                continue
            request_size = int(requests_label[2:])
            test_dir_full_name = '/'.join([tests_dir, test_dir_name])
            for _, _, log_files in os.walk(test_dir_full_name):
                client_reports = [parse_client_log('/'.join([test_dir_full_name, log_file])) for log_file in log_files]
                tests_data.add_client_reports(clients_count, request_size, client_reports)

    return tests_data


if __name__ == '__main__':
    parse_client_log(LOG_FILE)
    tests_data = parse_test_dir('remote_const')
    print(tests_data)

    # lines = [line.strip() for line in open(FILE_NAME).readlines() if len(line) > 0]
    # points = [line.split() for line in lines if not line.startswith("#") and len(line) > 0]
    #
    # xs = [float(point[0]) for point in points]
    # ys = [float(point[1]) for point in points]
    #
    # pdf = PdfPages('a.pdf')
    # fig = plt.figure(figsize=(3, 3))
    # plt.plot(xs, ys)
    # # plt.xlabel(X_LABEL)
    # # plt.ylabel(Y_LABEL)
    # pdf.savefig(fig)
    #
    # fig2 = plt.figure(figsize=(10, 10))
    # plt.plot([1, 1], [-1, 1])
    # pdf.savefig(fig2)
    #
    # pdf.close()

