import matplotlib.pyplot as plt

PYZX_LABEL="pyzx-0.7.3"
SHARQ_LABEL="sharq-0.0.4"

def main():

    with open("result_pyzx.csv", mode='r') as f:
        lines_pyzx = [s.rstrip() for s in f.readlines()]

    with open("result_sharq.csv", mode='r') as f:
        lines_sharq = [s.rstrip() for s in f.readlines()]

    names = []
    time_pyzx = []
    time_sharq = []
    tcount_pyzx = []
    tcount_sharq = []
    cxcount_pyzx = []
    cxcount_sharq = []
    gcount_pyzx = []
    gcount_sharq = []
    for line_pyzx, line_sharq in zip(lines_pyzx[1:], lines_sharq[1:]):
        token_pyzx = line_pyzx.split(',')
        token_sharq = line_sharq.split(',')
        names.append(token_pyzx[0])
        time_pyzx.append(float(token_pyzx[1]))
        tcount_pyzx.append(int(token_pyzx[3]))
        cxcount_pyzx.append(int(token_pyzx[5]))
        gcount_pyzx.append(int(token_pyzx[7]))
        time_sharq.append(float(token_sharq[1]))
        tcount_sharq.append(int(token_sharq[3]))
        cxcount_sharq.append(int(token_sharq[5]))
        gcount_sharq.append(int(token_sharq[7]))

    fig = plt.figure(figsize=[10,12])
    fig.subplots_adjust(top=0.95, bottom=0.185, hspace=0.65, wspace=0.3)

    # Optimization Time plot
    
    ax1 = fig.add_subplot(2, 2, 1)
    ax1.set_ylabel("Time (sec)", fontsize=14)
    g1 = ax1.bar(names, time_pyzx, label=PYZX_LABEL, align='edge', width=-0.3)
    g2 = ax1.bar(names, time_sharq, label=SHARQ_LABEL, align='edge', width=0.3)

    ax1.set_xticks(range(len(names)))
    ax1.set_xticklabels(names, rotation=90)
    ax1.set_yscale('log')
    ax1.legend(handles=[g1,g2], loc='upper left', fontsize=10)
    ax1.grid(axis='y')
    
    # T-Count plot

    ax2 = fig.add_subplot(2, 2, 2)
    ax2.set_ylabel("T-Count", fontsize=14)
    g1 = ax2.bar(names, tcount_pyzx, label=PYZX_LABEL, align='edge', width=-0.3)
    g2 = ax2.bar(names, tcount_sharq, label=SHARQ_LABEL, align='edge', width=0.3)

    ax2.set_ylim(0, 400)
    ax2.set_xticks(range(len(names)))
    ax2.set_xticklabels(names, rotation=90)
    ax2.legend(handles=[g1,g2], loc='upper left', fontsize=10)
    ax2.grid(axis='y')

    # 2Q-Count plot
    
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.set_ylabel("CNOT-Count", fontsize=14)
    g1 = ax3.bar(names, cxcount_pyzx, label=PYZX_LABEL, align='edge', width=-0.3)
    g2 = ax3.bar(names, cxcount_sharq, label=SHARQ_LABEL, align='edge', width=0.3)

    ax3.set_xticks(range(len(names)))
    ax3.set_xticklabels(names, rotation=90)
    ax3.legend(handles=[g1,g2], loc='upper left', fontsize=10)
    ax3.grid(axis='y')
    
    # G-Count plot
    
    ax4 = fig.add_subplot(2, 2, 4)
    ax4.set_ylabel("Gate-Count", fontsize=14)
    g1 = ax4.bar(names, gcount_pyzx, label=PYZX_LABEL, align='edge', width=-0.3)
    g2 = ax4.bar(names, gcount_sharq, label=SHARQ_LABEL, align='edge', width=0.3)

    ax4.set_ylim(0, 3500)
    ax4.set_xticks(range(len(names)))
    ax4.set_xticklabels(names, rotation=90)
    ax4.legend(handles=[g1,g2], loc='upper left', fontsize=10)
    ax4.grid(axis='y')

    # show and save
    
    #fig.tight_layout()
    plt.show()
    fig.savefig("plot.png")
    
if __name__ == '__main__':
    main()

