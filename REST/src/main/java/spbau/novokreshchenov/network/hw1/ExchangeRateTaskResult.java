package spbau.novokreshchenov.network.hw1;


public class ExchangeRateTaskResult {
    private final String bankName;
    private final String currency;
    private final float buyValue;
    private final float sellValue;

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
}
