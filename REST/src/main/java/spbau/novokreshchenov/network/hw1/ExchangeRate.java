package spbau.novokreshchenov.network.hw1;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ExchangeRate {
    @XmlElement(name="BUY")
    private final float buyValue;
    @XmlElement(name="SELL")
    private final float sellValue;

    public ExchangeRate(float buyValue, float sellValue) {
        this.buyValue = buyValue;
        this.sellValue = sellValue;
    }

    public float getBuyValue() {
        return buyValue;
    }

    public float getSellValue() {
        return sellValue;
    }
}
