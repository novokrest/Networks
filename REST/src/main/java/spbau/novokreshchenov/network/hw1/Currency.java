package spbau.novokreshchenov.network.hw1;


public class Currency {
    private final String name;
    private final String iconPath;

    public Currency(String name, String iconPath) {
        this.name = name;
        this.iconPath = iconPath;
    }

    public String getName() {
        return name;
    }

    public String getIconPath() {
        return iconPath;
    }
}
