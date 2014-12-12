package spbau.novokreshchenov.network.hw1.Tasks;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class PutResult {
    public String id;

    public PutResult() {}

    public PutResult(String id) {
        this.id = id;
    }
}
