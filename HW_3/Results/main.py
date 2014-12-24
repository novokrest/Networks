__author__ = 'novokreshchenov.konstantin'

import os
from matplotlib.backends.backend_pdf import PdfPages
from pylab import *


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

    def clients_count(self):
        return len(self.data.values())


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


TEST_DIR_CONST_REQUESTS = 'remote_const'
TEST_DIR_RANDOM_REQUESTS = 'remote_random1'
PDF_RESULTS = 'results.pdf'
PDF_RESULTS_RANDOM = 'results_random.pdf'

REQUEST_SIZES = [10 ** i for i in range(7)]

fig = 1


def make_results():
    global fig
    tests_data = parse_test_dir(TEST_DIR_CONST_REQUESTS)
    pdf = PdfPages(PDF_RESULTS)
    for rs in REQUEST_SIZES:
        xs = []
        ys_total = []
        ys_generate = []
        ys_connection = []
        ys_send = []
        ys_receive = []
        for i in range(1, tests_data.clients_count() + 1):
            client_reports = tests_data.data[i][rs]
            xs.append(i)
            ys_total.append(sum([client_report.total_time for client_report in client_reports]) / i)
            ys_generate.append(sum([client_report.time_generating_request for client_report in client_reports]) / i)
            ys_connection.append(sum([client_report.time_connection_to_server for client_report in client_reports]) / i)
            ys_send.append(sum([client_report.time_sending_request for client_report in client_reports]) / i)
            ys_receive.append(sum([client_report.time_receiving_response for client_report in client_reports]) / i)
        figure(fig, figsize=(10, 20))
        title('REQUEST = {0} bytes'.format(rs))
        xlabel('CLIENTS')
        ylabel('TIME (ms)')

        plt_total, = plot(xs, ys_total)
        plt_generate, = plot(xs, ys_generate)
        plt_connection, = plot(xs, ys_connection)
        plt_send, = plot(xs, ys_send)
        plt_receive, = plot(xs, ys_receive)

        legend([plt_total, plt_generate, plt_connection, plt_send, plt_receive],
               ['Total time', 'Generating request', 'Connection', "Sending request", "Waiting response"])
        pdf.savefig()
        fig += 1

    pdf.close()


def make_results_random():
    global fig
    tests_data = parse_test_dir(TEST_DIR_RANDOM_REQUESTS)
    pdf = PdfPages(PDF_RESULTS_RANDOM)
    xs = []
    ys_total = []
    ys_generate = []
    ys_connection = []
    ys_send = []
    ys_receive = []
    for i in range(1, tests_data.clients_count() + 1):
        client_reports = tests_data.data[i][0]
        xs.append(i)
        ys_total.append(sum([client_report.total_time for client_report in client_reports]) / i)
        ys_generate.append(sum([client_report.time_generating_request for client_report in client_reports]) / i)
        ys_connection.append(sum([client_report.time_connection_to_server for client_report in client_reports]) / i)
        ys_send.append(sum([client_report.time_sending_request for client_report in client_reports]) / i)
        ys_receive.append(sum([client_report.time_receiving_response for client_report in client_reports]) / i)
    figure(fig, figsize=(10, 20))
    title('REQUEST = Rand(8..200008) bytes'.format(0))
    xlabel('CLIENTS')
    ylabel('TIME (ms)')

    plt_total, = plot(xs, ys_total)
    plt_generate, = plot(xs, ys_generate)
    plt_connection, = plot(xs, ys_connection)
    plt_send, = plot(xs, ys_send)
    plt_receive, = plot(xs, ys_receive)

    legend([plt_total, plt_generate, plt_connection, plt_send, plt_receive],
           ['Total time', 'Generating request', 'Connection', "Sending request", "Waiting response"])
    pdf.savefig()

    pdf.close()


if __name__ == '__main__':
    make_results()
    make_results_random()




