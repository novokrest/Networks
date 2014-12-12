package spbau.novokreshchenov.network.hw1.Tasks;

import javax.xml.bind.annotation.XmlRootElement;


@XmlRootElement
public class PutTask {
    public String bank;
    public String currency;

    public PutTask() {}

    public PutTask(String bank, String currency) {
        this.bank = bank;
        this.currency = currency;
    }
}
