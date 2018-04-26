//'main' method must be in a class 'Rextester'.
//Compiler version 1.8.0_111

import java.util.*;
import java.lang.*;

class Rextester
{
  interface IToken {
    String text();
        void setText(String text);
    int length();
  }

  static class WordToken implements IToken {
    private String word;

    public WordToken(String word) {
      this.word = word;
    }

    public String text() {
      return word;
    }

        public void setText(String text) {
            word = text;
        }

    public int length() {
      return word.length();
    }
  }

  interface ITextHandler {
    void Process(IToken text);
  }

  static class LowercaseHandler implements ITextHandler {
    public void Process(IToken token) {
      token.setText(token.text().toLowerCase());
    }
  }

    static class Array<T extends ITextHandler> {
    private static final int initialCapacity = 4;
    private T[] buffer;
    private int size = 0;
    private int capacity = initialCapacity;

        public Array() {
            buffer = (T[]) new ITextHandler[capacity];
        }

        public void add(T element) {
            buffer[size] = element;
            ++size;
            tryResize();
        }

        public int size() {
            return size;
        }

        public T get(int index) {
            return buffer[index];
        }

    public void apply(List<? extends IToken> tokens) {
            for (IToken token : tokens) {
                for (int i = 0; i < size; ++i) {
                    buffer[i].Process(token);
                }
            }
    }

        private void tryResize() {
            if( size != capacity ) {
                return;
            }

            int newCapacity = 2 * capacity;
            T[] newBuffer = (T[]) new ITextHandler[newCapacity];

            for( int i = 0; i < capacity; ++i ) {
                newBuffer[i] = buffer[i];
            }

            capacity = newCapacity;
            buffer = newBuffer;
        }
  }

    private static void initTokens(List<? super WordToken> tokens) {
    tokens.add(new WordToken("Мама"));
    tokens.add(new WordToken("мыла"));
    tokens.add(new WordToken("раму"));
    }

  public static void main(String[] args) throws java.lang.Exception {
        List<IToken> tokens = new ArrayList<IToken>();
    initTokens(tokens);

    Array<ITextHandler> handlers = new Array<>();
    handlers.add(new LowercaseHandler());

    handlers.apply(tokens);
    System.out.println(tokens.get(0).text());
  }
}
