package spbau.novokreshchenov.network.hw1.Tasks;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;


@XmlRootElement
public class PutTask {
    @XmlElement(name="bank")
    public String bank;
    @XmlElement(name="currency")
    public String currency;

    public PutTask() {}

    public PutTask(String bank, String currency) {
        this.bank = bank;
        this.currency = currency;
    }
}
