package spbau.novokreshchenov.network.hw1;


public class ExchangeRateTask {
    private final String bankName;
    private final String currency;

    public ExchangeRateTask(String bankName, String currency) {
        this.bankName = bankName;
        this.currency = currency;
    }

    public String getBankName() {
        return bankName;
    }

    public String getCurrency() {
        return currency;
    }
}
