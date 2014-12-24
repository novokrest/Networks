package spbau.novokreshchenov.network.hw1;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class ExchangeRateTaskResult {
    public String bankName;
    public String currency;
    public float buyValue;
    public float sellValue;

    public ExchangeRateTaskResult() {}

    public ExchangeRateTaskResult(ExchangeRateTask task, float buyValue, float sellValue) {
        this.bankName = task.getBankName();
        this.currency = task.getCurrency();
        this.buyValue = buyValue;
        this.sellValue = sellValue;
    }

    public String getBankName() {
        return bankName;
    }

    public String getCurrency() {
        return currency;
    }

    public float getBuyValue() {
        return buyValue;
    }

    public float getSellValue() {
        return sellValue;
    }

    @Override
    public String toString() {
        return "bankName: " + bankName
                + "currency: " + currency
                + "buyValue: " + buyValue
                + "sellValue: " + sellValue;
    }
}
