package spbau.novokreshchenov.network.hw1.Exceptions;


import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name="exception")
public class XmlMessageException {
    public String message;

    public XmlMessageException() {}

    public XmlMessageException(String message) {
        this.message = message;
    }
}
